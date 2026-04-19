#include "OV7670.h"
#include <math.h>

static char debug_i2c[255];

int8_t config_vsyn(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    //__HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    return 0;
}

int8_t config_pixels(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct_pixel;

    GPIO_InitStruct_pixel.Pin = GPIO_PIN_4;
    GPIO_InitStruct_pixel.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct_pixel.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_pixel); 
        
    GPIO_InitStruct_pixel.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_pixel); 

    GPIO_InitStruct_pixel.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_pixel); 

    GPIO_InitStruct_pixel.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_pixel); 


    GPIO_InitStruct_pixel.Pin = GPIO_PIN_12;
    GPIO_InitStruct_pixel.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct_pixel.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_pixel); 
        
    GPIO_InitStruct_pixel.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_pixel); 

    GPIO_InitStruct_pixel.Pin = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_pixel); 

    GPIO_InitStruct_pixel.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_pixel); 

    return 0;
}

int8_t ov7670_init(void)
{
    static uint8_t COM7[]= {0x12,0x06}; //00000110

    if(i2c_send(OV7670_Addr, COM7, 2))
    {
        strcpy(debug_i2c,"Error config camera\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }
    return 0;
}

