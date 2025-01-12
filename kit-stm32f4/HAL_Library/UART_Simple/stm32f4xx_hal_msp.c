/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComIT/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.    
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @defgroup UART_TwoBoards_ComIT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
      //USARTx_TX_GPIO_CLK_ENABLE();
      //USARTx_RX_GPIO_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /* Enable USART2 clock */
  __HAL_RCC_USART2_CLK_ENABLE();  //USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_2; //USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;  //USARTx_TX_AF;
  
  HAL_GPIO_Init(GPIOA /*USARTx_TX_GPIO_PORT*/, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = GPIO_PIN_3;  //USARTx_RX_PIN;
  //GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
  HAL_GPIO_Init(GPIOA/*USARTx_RX_GPIO_PORT*/, &GPIO_InitStruct);
    
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(USART2_IRQn/*USARTx_IRQn*/, 0, 1);
  HAL_NVIC_EnableIRQ(USART2_IRQn/*USARTx_IRQn*/);
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_USART2_FORCE_RESET();//USARTx_FORCE_RESET();
  __HAL_RCC_USART2_RELEASE_RESET(); //USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure UART Tx as alternate function */
  HAL_GPIO_DeInit(GPIOA /*USARTx_TX_GPIO_PORT*/, GPIO_PIN_2 /*USARTx_TX_PIN*/);
  /* Configure UART Rx as alternate function */
  HAL_GPIO_DeInit(GPIOA /*USARTx_RX_GPIO_PORT*/, GPIO_PIN_3 /*USARTx_RX_PIN*/);
  
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USART2_IRQn/*USARTx_IRQn*/);
}

