#include "config_usart.h"


UART_HandleTypeDef UartHandle;

void USART_config(void)
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

void BSP_LED_Init()
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  // Enable the GPIO_LED Clock //
  __HAL_RCC_GPIOC_CLK_ENABLE();

  // Configure the GPIO_LED pin //
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  //GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); 
}

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /*        Internal crystal    
  * After each device reset the HSI (16 MHz) is used as system clock source.
  * Then SystemInit() function is called, in "startup_stm32f4xx.s" file, to
  * configure the system clock before to branch to main program.
  */
  /*
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
  */
 /* Enable HSE oscillator and configure the PLL to reach the max system frequency (100 MHz)
     when using HSE oscillator as PLL clock source. 
  */
  
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  /* PLL configuration:
     HSE = 8 MHz
     PLLM = 8    → 1 MHz
     PLLN = 336  → 336 MHz
     PLLP = 2    → 168 MHz SYSCLK
     PLLQ = 7    → 48 MHz (USB, RNG, etc.)
  */
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2; // 200 / 2 = 100 MHz
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    //Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  // 50 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  // 100 MHz
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    //Error_Handler();
  }

}

int8_t usart_send(uint8_t *buffer, int len)
{  
  if (HAL_UART_Transmit(&UartHandle, buffer, len, 5000) != HAL_OK)
    return -1;
  
  return 0;
}
 