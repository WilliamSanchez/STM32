#include "config_xclk.h"

TIM_HandleTypeDef    TimHandle;

uint32_t uwPrescalerValue = 0;

void pinout_xclk_init()
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  // Enable the GPIO_LED Clock //
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // Configure the GPIO_LED pin //
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); 
}

int8_t begin_xclk(){
    
    /* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz 
    
        Divide the timer-2 input frequency (16Mhz)
        by a factor of 1000 (16,000,000/1,000 = 16,000 = 16Khz) 
    */
    uwPrescalerValue = (uint32_t) ((SystemCoreClock / 1600000) - 1);
    TimHandle.Instance = TIM;
    /*
        We want the time count to be 500msec (half a second).
        As the input frequency is 16khz so the total
        counts required for 500msec delay:
        
        total counts = 500msec * f
                     = (.5 sec) * 16,000
                     = 8,000
                     = 0x1F40
    */
    TimHandle.Init.Period = 2;//0xFFFFFFFF;//
    TimHandle.Init.Prescaler = 0;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    //TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
        return -1;

    //if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    //    return -1;

    if(HAL_TIM_OC_Init(&TimHandle) != HAL_OK)
        return -1;

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
    sConfigOC.Pulse = 0;                   // toggle every tick
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;

    HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_3);

    pinout_xclk_init();

    return 0;
}

