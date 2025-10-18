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

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();
  GPIO_InitStruct.Pin       = GPIO_PIN_9; //USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;  //USARTx_TX_AF;
  
  HAL_GPIO_Init(GPIOA /*USARTx_TX_GPIO_PORT*/, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_10;  /
  HAL_GPIO_Init(GPIOA/*USARTx_RX_GPIO_PORT*/, &GPIO_InitStruct);

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_SPI1_CLK_ENABLE();  
  
  GPIO_InitStruct.Pin       = GPIO_PIN_5;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_6;  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        
  
  GPIO_InitStruct.Pin = GPIO_PIN_7;    
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_USART1_FORCE_RESET();//USARTx_FORCE_RESET();
  __HAL_RCC_USART1_RELEASE_RESET(); //USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure UART Tx as alternate function */
  HAL_GPIO_DeInit(GPIOA /*USARTx_TX_GPIO_PORT*/, GPIO_PIN_9 /*USARTx_TX_PIN*/);
  /* Configure UART Rx as alternate function */
  HAL_GPIO_DeInit(GPIOA /*USARTx_RX_GPIO_PORT*/, GPIO_PIN_10 /*USARTx_RX_PIN*/);
  
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USART2_IRQn/*USARTx_IRQn*/);
}
