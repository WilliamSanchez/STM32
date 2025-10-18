/**
  ******************************************************************************
  * @file    GPIO/GPIO_EXTI/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use GPIOs through 
  *          the STM32F4xx HAL API.
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

#include <string.h>
#include <stdio.h>

#define USART                         USART1
/*
#define USART_CLK_ENABLE()            __HAL_RCC_USART1_CLK_ENABLE()
#define USART_RX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_TX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define USART_FORCE_RESET()           __HAL_RCC_USART1_FORCE_RESET()
#define USART_RELEASE_RESET()         __HAL_RCC_USART1_RELEASE_RESET()

#define USART_TX_PIN                    GPIO_PIN_9
#define USART_TX_GPIO_PORT              GPIOA  
#define USART_TX_AF                     GPIO_AF7_USART1
#define USART_RX_PIN                    GPIO_PIN_10
#define USART_RX_GPIO_PORT              GPIOA 
#define USART_RX_AF                     GPIO_AF7_USART1
*/

static void SystemClock_Config(void);
//static void EXTILine0_Config(void);
static void BSP_LED_Init();
static void USART_config(void);
static void SPI_config(void);

UART_HandleTypeDef UartHandle;
SPI_HandleTypeDef SpiHandle;

__IO ITStatus UartReady = RESET;

uint8_t aTxBuffer[16] = "HOLA\n\r";


/*

  PA0   => Switch key0
  PC13  => Led  

*/

int main(void)
{

  HAL_Init();

    /* Configure the system clock to 100 MHz */
  SystemClock_Config();

  USART_config();
  SPI_config();
 
  /* Configure LED3, LED4, LED5 and LED6 */
  BSP_LED_Init();
    
  /* Configure EXTI Line0 (connected to PA0 pin) in interrupt mode */


  HAL_Delay(1000);
  uint8_t rest;
  if ((rest = HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, strlen((char* )aTxBuffer), 5000)) != HAL_OK)
  {
    sprintf((char*)aTxBuffer,"RE %x\n\r",rest);
    HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, 4, 5000);
  }
  
  /* Infinite loop */
  uint8_t data[4];
  uint8_t command = 0x04;
  while (1)
  {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
      HAL_SPI_Transmit(&SpiHandle, &command, 1, 5000); 
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
      HAL_SPI_Receive(&SpiHandle, data, 4, 5000);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
      HAL_Delay(1000);
      sprintf((char*)aTxBuffer,"DATA %x|%x|%x|%x\n\r",data[0],data[1],data[2],data[3]);
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      if ((rest = HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, strlen((char* )aTxBuffer), 5000)) != HAL_OK)
      {
        sprintf((char*)aTxBuffer,"RE %x\n\r",rest);
        HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, 4, 5000);
      }
      HAL_Delay(1000);
  }
}


static void USART_config(void)
{

    UartHandle.Instance           = USART;

    UartHandle.Init.BaudRate      = 115200;
    UartHandle.Init.WordLength    = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits      = UART_STOPBITS_1;
    UartHandle.Init.Parity        = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl     = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode          = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling  = UART_OVERSAMPLING_16;

    while(HAL_UART_Init(&UartHandle) != HAL_OK);

}


static void SPI_config(void)
{

    SpiHandle.Instance           = SPI1;

    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_HARD_OUTPUT;//SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;

    SpiHandle.Init.Mode              = SPI_MODE_MASTER;

    while(HAL_SPI_Init(&SpiHandle) != HAL_OK);

}


static void BSP_LED_Init()
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_10;  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_4;  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET); 
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    //Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    //Error_Handler();
  }
}
