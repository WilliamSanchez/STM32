/**
  ******************************************************************************
  * @file    GPIO/GPIO_Toggle/stm32f30x_it.c 
  * @author  MCD Application Team
  * @version V1.1.2
  * @date    14-August-2015
  * @brief   Main Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_it.h"

#include <string.h>
#include <stdio.h>

#include <modulo3G.h>
#include <misc.h>

#define muestras 100
/** @addtogroup STM32F30x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/


extern char trasmitData[1024];
extern uint8_t memoria1[1024];
extern uint8_t memoria2[1024];

extern uint8_t dataGPS;

uint8_t rxBuffer;
uint8_t txBuffer;




//uint8_t rxCmd = 0x00;
//uint8_t command;
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles External lines 5 to 9 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{ 
   if((EXTI_GetITStatus(EXTI_Line0) != RESET)){

       
       GPIO_ResetBits(GPIOE,GPIO_Pin_10);
       sprintf(trasmitData,"10:45:32:54 10:45:35:45 -70.12345 -23.98545 20 23 20 19\n\r");
       USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
       while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
       
   }
   EXTI_ClearITPendingBit(EXTI_Line0);
}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/******************************************************************************/
/**
  * @brief  This function handles TIM16 global interrupt.
  * @param  None
  * @retval : None
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  
}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/******************************************************************************/
/**
  * @brief  This function handles COMP interrupt request.
  * @param  None
  * @retval None
  */
void COMP4_5_6_IRQHandler(void)
{
}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles External lines 5 to 9 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{ 
 
}

/*****************************************************************************/
/*	       STM32F30x	Serial Interrupt  	USART 1		     */
/*****************************************************************************/

void USART1_IRQHandler(void){	    ///// ---------------------->>>>>>>>>>>>  M10
  
  static int tx_index = 0;
  static int rx_index = 0;

    //GPIO_ResetBits(GPIOB,GPIO_Pin_4);

    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) // Transmit the string in a loop
    {
        GPIO_ResetBits(GPIOE,GPIO_Pin_11);        
        USART_SendData(USART1, *(memoria1+tx_index));
	tx_index++;
    	if (tx_index == strlen((char*)memoria1)){
    	    tx_index = 0;	
	    memset(memoria1,0x00,strlen((char*)memoria1));	    
    	    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	    USART_Cmd(USART2, ENABLE);
	 }
    }
/*
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // Received characters modify string
    {
        rxBuffer=USART_ReceiveData(USART1);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET));
    }
*/
}

/*****************************************************************************/
/*	       STM32F30x	Serial Interrupt  	USART  2             */
/*****************************************************************************/

void USART2_IRQHandler(void){   ///// ---------------------->>>>>>>>>>>>  PC

  static uint8_t tx_index = 0;
  static uint8_t rx_index = 0;
  static uint8_t index = 0;

  // GPIO_ResetBits(GPIOB,GPIO_Pin_4);

    if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) // Transmit the string in a loop
    {
        USART_SendData(USART2, rxBuffer);	    
    	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    }

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // Received characters modify string
    {
        txBuffer=USART_ReceiveData(USART2);

        *(memoria2+rx_index++) = txBuffer;
	if(parseData((char*)memoria2,rx_index,"$GPGLL,")){
           if(txBuffer == ',') txBuffer =' ';
           *(trasmitData+index++) = txBuffer;
           if(txBuffer == '\n'){
 	      GPIO_ResetBits(GPIOE,GPIO_Pin_10);txBuffer=0x00;
	      rx_index = 0; index = 0;  dataGPS = 1;
	      USART_Cmd(USART2, DISABLE);
           }     
        }

    }

}

/*****************************************************************************/
/*	       STM32F30x	Serial Interrupt  	USART  2             */
/*****************************************************************************/

void USART3_IRQHandler(void){   ///// ---------------------->>>>>>>>>>>>  PC
/*
  static int tx_index = 0;
  static int rx_index = 0;

  // GPIO_ResetBits(GPIOB,GPIO_Pin_4);

    if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) // Transmit the string in a loop
    {
        GPIO_ResetBits(GPIOE,GPIO_Pin_10);
        USART_SendData(USART3, *(trasmitData+tx_index));
	tx_index++;
    	if (tx_index >= strlen(trasmitData)){
    	    tx_index = 0;	
	    memset(trasmitData,0x00,strlen(trasmitData));	    
    	    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	 }
    }

    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // Received characters modify string
    {
         char Buffer = USART_ReceiveData(USART3);
        *(bleErrorBLE+rx_index++) = Buffer;
       
        if(parseData((char*)bleErrorBLE, rx_index, "OK")){	
	   GPIO_ResetBits(GPIOE,GPIO_Pin_11); 
           memset((char*)bleErrorBLE,0x00,strlen((char*)bleErrorBLE));rx_index = 0;
           	   		
	} 	
         else if(parseData((char*)bleErrorBLE, rx_index, "ERROR")){ 
           memset((char*)bleErrorBLE,0x00,strlen((char*)bleErrorBLE)); rx_index = 0;               		
	}

    }
*/
}


/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/




