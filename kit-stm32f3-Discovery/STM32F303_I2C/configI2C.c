#include "configI2C.h"


#define VEML_6070_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define VEML_6070_LONG_TIMEOUT         ((uint32_t)(10 * VEML_6070_FLAG_TIMEOUT))  
#define I2C_TIMEOUT         ((uint32_t)0x3FFFF) /*!< I2C Time out */
/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER     300

I2C_InitTypeDef I2C_InitStruct;

__IO uint32_t  VEML_6070_Timeout = VEML_6070_LONG_TIMEOUT;

void Init_I2C1(void){

	
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
   RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOB, ENABLE);

    /* Connect PXx to I2C_SCL */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_4);
   /* Connect PXx to I2C_SDA */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_4);

   /* Configure TS751_I2C pins: SCL */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* Configure TS751_I2C pins: SDA */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
   I2C_InitStruct.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
   I2C_InitStruct.I2C_DigitalFilter = 0x00;
   I2C_InitStruct.I2C_OwnAddress1 = 0x00;
   I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
   I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
   I2C_InitStruct.I2C_Timing = 0xC062121F;

   I2C_Init(I2C1, &I2C_InitStruct);
   I2C_Cmd(I2C1, ENABLE);

}



/////////////////////   Write comand     ///////////////////// 


int I2C_WriteReg(uint8_t RegValue){

  uint8_t buffer_aux;
  buffer_aux = RegValue;


  VEML_6070_Timeout = VEML_6070_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET)
  {
    if((VEML_6070_Timeout--) == 0) return 1;
  }

  // Configure slave address, nbytes, reload, end mode and start or stop generation 
  I2C_TransferHandling(I2C1,(Add_I2C<<1), 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);  //VEML_6070_ADDR

  // Wait until TXIS flag is set 
  VEML_6070_Timeout = VEML_6070_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET)   
  {
    if((VEML_6070_Timeout--) == 0) return 2;
  }  
  
  // Write data to TXDR 
  I2C_SendData(I2C1, (uint8_t)buffer_aux);

  // Wait until STOPF flag is set 
  VEML_6070_Timeout = VEML_6070_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET)
  {
    if((VEML_6070_Timeout--) == 0) return 3;
  }   
  
  // Clear STOPF flag 
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  I2C1_clearACK();

  return 0;
}




////////////   Read


uint8_t I2C1_ReadData(void)
{   

  uint8_t tmp = 0;
  
  /**************** Read Temperature value high Register ***********************/
  /* Test on BUSY Flag */
  VEML_6070_Timeout = VEML_6070_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET)
  {
    if((VEML_6070_Timeout--) == 0) return 4;
  }     
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C1, (Add_I2C<<1) | 0x01, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read); 
  /* Wait until RXNE flag is set */
  VEML_6070_Timeout = VEML_6070_LONG_TIMEOUT;    
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET)
  {
    if((VEML_6070_Timeout--) == 0) return 5;
  } 
  /* Read data from RXDR */
  tmp = I2C_ReceiveData(I2C1); 
  /* Wait until STOPF flag is set */
  VEML_6070_Timeout = VEML_6070_LONG_TIMEOUT; 
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET)
  {
    if((VEML_6070_Timeout--) == 0) return 6;
  }
  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);  


  return tmp;
}


/////////////////   Ready


int I2C1_clearACK(void)      
{
  __IO uint32_t sEETrials = 0;
  
  /* Keep looping till the slave acknowledge his address or maximum number 
  of trials is reached (this number is defined by sEE_MAX_TRIALS_NUMBER define
  in stm32303c_eval_i2c_ee.h file) */
  
  /* Configure CR2 register : set Slave Address and end mode */
  I2C_TransferHandling(I2C1, (Add_I2C<<1), 0, I2C_AutoEnd_Mode, I2C_No_StartStop);  
  
  do
  { 
    /* Initialize sEETimeout */
    VEML_6070_Timeout = VEML_6070_LONG_TIMEOUT;
    
    /* Clear NACKF */
    I2C_ClearFlag(I2C1, I2C_ICR_NACKCF | I2C_ICR_STOPCF);
    
    /* Generate start */
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Wait until timeout elapsed */
    while (VEML_6070_Timeout-- != 0); 
    
    /* Check if the maximum allowed numbe of trials has bee reached */
    if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
    {
      /* If the maximum number of trials has been reached, exit the function */
      return 1;
    }
  }
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) != RESET);
  
  /* Clear STOPF */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
  
  /* Return sEE_OK if device is ready */
  return 0;
}
