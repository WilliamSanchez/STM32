#include "OV7670.h"
#include <math.h>

static uint8_t ID_MSB = 0x0A;
static uint8_t COM14[] = {0x3E, 0x1C};
static uint8_t SCL_PCLK[] = {0x73, 0x04};
static uint8_t CLKRC[] = {0x11, 0x03F};


static char debug_i2c[255];

int8_t config_pclk(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    //__HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
    HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    return 0;
}

/*

  PA1   =>  BIT0
  PA2   =>  BIT1
  PA3   =>  BIT2
  PA6   =>  BIT3 //MISO  Optional
  PB12  =>  BIT4
  PB13  =>  BIT5
  PB14  =>  BIT6
  PB15  =>  BIT7

*/

int8_t config_href(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();
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

    GPIO_InitStruct_pixel.Pin = GPIO_PIN_1;
    GPIO_InitStruct_pixel.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct_pixel.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_pixel); 
        
    GPIO_InitStruct_pixel.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_pixel); 

    GPIO_InitStruct_pixel.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_pixel); 

    GPIO_InitStruct_pixel.Pin = GPIO_PIN_6;
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
    
    static uint8_t ID[2];

    if(i2c_received(OV7670_Addr, &ID_MSB, ID, 2) > 0)
    {
        sprintf(debug_i2c,"ID %x%x\n\r",ID[1], ID[0]);
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
    }

    if(i2c_send(OV7670_Addr, COM14, 2) < 0)
    {
        strcpy(debug_i2c,"Error config camera\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }

    if(i2c_send(OV7670_Addr, SCL_PCLK, 2) < 0)
    {
        strcpy(debug_i2c,"Error config camera\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }

    if(i2c_send(OV7670_Addr, CLKRC, 2) < 0)
    {
        strcpy(debug_i2c,"Error config camera\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }

    if(i2c_send(OV7670_Addr, COM7, 2))
    {
        strcpy(debug_i2c,"Error config camera\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }
    return 0;
}

