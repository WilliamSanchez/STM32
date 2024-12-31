/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"
#include <string.h>
#include <stdio.h>

/** @addtogroup Template_Project
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

extern char txBuffer_1[];
extern uint8_t changeMode[];
extern char dataMode[];
extern uint8_t mode;
uint8_t cont = 0;
extern uint8_t contDelay;

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
//void SVC_Handler(void)
//{
//}

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
//void PendSV_Handler(void)
//{
//}
/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
/*
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    // Toggle LED1 //

    memset(dataMode,0x00,225);
    if (mode == 0){
       sprintf(dataMode,"If you want to master English, get involved and practise as much as possible.");
    }else if(mode == 1){
      sprintf(dataMode," END DEVICE Walt Disney was well known as a man who made dreams come true, and you can, too. It just takes plenty of hard work.");
    }else{
      sprintf(dataMode,"END DEVICE Learning a skill such as a new language can take a long time. If you feel like your progress is slow, bear in mind these wise words from Confucius. The important thing is to keep going and you will get there in the end.");
    }
    sprintf(changeMode,"MODE [ %d ] %d",mode%3,strlen(dataMode));
    // Clear the EXTI line 0 pending bit //
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
  */
}

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
  /*
  if(EXTI_GetITStatus(EXTI_Line4) != RESET)
  {
     Toggle LED1 
    if (cont++ > 3) cont = 0;
    mode = cont%3;
    sprintf(changeMode,"MODE %d    ",mode);
    EXTI_ClearITPendingBit(EXTI_Line4);
  }*/
}

/**
  * @brief  This function handles External line 0 interrupt request.

  * @param  None
  * @retval None
  */


void EXTI3_IRQHandler(void)
{
  /*
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)
  {
    // Toggle LED1 //
    if (contDelay++ > 12) contDelay = 1;
    //sprintf(changeDelay,"    [ %ds]    ",mode);
    EXTI_ClearITPendingBit(EXTI_Line3);
  }*/
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{

//}

/*****************************************************************************/
/*	       STM32F30x	Serial Interrupt  	USART 1		     */
/*****************************************************************************/

void USART1_IRQHandler(void){	    ///// ---------------------->>>>>>>>>>>>  M10
/*
static int tx_index = 0;

    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) // Transmit the string in a loop
    {
        USART_SendData(USART1, *(txBuffer_1+tx_index));
	tx_index++;
    	if (tx_index == strlen(txBuffer_1)){
    	    tx_index = 0;	    
    	    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	   }
    }

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // Received characters modify string
    {
        char data1=USART_ReceiveData(USART1);
        *(memoria1+rx_index++) = data1;
	if(parseData((char*)memoria1, rx_index, "CMTI")){
	   GPIO_ResetBits(GPIOE,GPIO_Pin_11); 
	   rx_index = 0;
	   rxCommand = 1;	   
	   USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
           while ((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)); 
        }
	  else if(parseData((char*)memoria1, rx_index, "OK")){
	   GPIO_ResetBits(GPIOE,GPIO_Pin_11); 
	   rx_index = 0;
	   rxData = 1;	   
	   USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
           while ((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET));		
	}	
    }
*/
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
