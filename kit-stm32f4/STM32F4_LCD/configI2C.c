#include "configI2C.h"


#define SI2C_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SI2C_LONG_TIMEOUT         ((uint32_t)(10 * SI2C_FLAG_TIMEOUT))  
#define SI2C_TIMEOUT         ((uint32_t)0x3FFFF) /*!< I2C Time out */
/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define TRIALS_NUMBER     300

I2C_InitTypeDef I2C_InitStructure;

__IO uint32_t  I2C_Timeout = SI2C_LONG_TIMEOUT;

void Init_I2C1(void){

	
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Connect PXx to I2C_SCL */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
   /* Connect PXx to I2C_SDA */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

   /* Configure TS751_I2C pins: SCL */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* Configure TS751_I2C pins: SDA */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
   I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
   I2C_InitStructure.I2C_OwnAddress1 = 0x00;
   I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
   I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
   I2C_InitStructure.I2C_ClockSpeed = 100000;

   I2C_Init(I2C1, &I2C_InitStructure);
   I2C_Cmd(I2C1, ENABLE);

}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/////////////////////   Write register	 /////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// 



uint8_t I2C_WriteBuffer(uint8_t *buffer, uint8_t *length){

  uint32_t DataNum = 0;

  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((I2C_Timeout--) == 0) return 1;
  }


  /*!< Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /*!< Test on EV5 and clear it */
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB))
  {
    if((I2C_Timeout--) == 0) return 2;
  }

  /*!< SendOLED address for write */
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  I2C_Send7bitAddress(I2C1, Add_I2C,        I2C_Direction_Transmitter);

// Wait //
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR))
  {
    if((I2C_Timeout--) == 0) return 3;
  }

  /* Read status register 2 to clear ADDR flag */
  I2C1->SR2;

// Wait //
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE))
  {
    if((I2C_Timeout--) == 0) return 4;
  }

  /*!< Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(I2C1, 0x40);

  /*!< Test on EV8 and clear it */
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE))
  {
    if((I2C_Timeout--) == 0) return 5;
  }

  while(DataNum < *length){

     while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET);
     // Write data to TXDR 
     I2C_SendData(I2C1, (uint8_t)(*(buffer+DataNum)));
     DataNum++;
  }


  /* End the configuration sequence */
  I2C_GenerateSTOP(I2C1, ENABLE);

  return 0;

}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/////////////////////   Write register	 /////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
 


uint8_t I2C_WriteReg(uint8_t RegValue){

  static uint8_t buffer_aux;
  buffer_aux = RegValue;


  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((I2C_Timeout--) == 0) return 1;
  }


  /*!< Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /*!< Test on EV5 and clear it */
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB))
  {
    if((I2C_Timeout--) == 0) return 2;
  }

  /*!< SendOLED address for write */
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  I2C_Send7bitAddress(I2C1, Add_I2C,        I2C_Direction_Transmitter);

// Wait //
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR))
  {
    if((I2C_Timeout--) == 0) return 3;
  }

  /* Read status register 2 to clear ADDR flag */
  I2C1->SR2;

// Wait //
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE))
  {
    if((I2C_Timeout--) == 0) return 4;
  }

  /*!< Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(I2C1, 0x00);

  /*!< Test on EV8 and clear it */
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE))
  {
    if((I2C_Timeout--) == 0) return 5;
  }

  /*!< Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(I2C1, buffer_aux);

  /*!< Test on EV8 and clear it */
  I2C_Timeout = SI2C_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE))
  {
    if((I2C_Timeout--) == 0) return 6;
  }

  /* End the configuration sequence */
  I2C_GenerateSTOP(I2C1, ENABLE);

  return 0;
}

