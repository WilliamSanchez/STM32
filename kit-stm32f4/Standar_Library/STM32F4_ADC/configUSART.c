#include <configUSART.h>

#include <string.h>
#include <stdio.h>

#include "misc_extras.h"

uint8_t trasmitDataS[255];

__IO uint16_t ADC_Value = 0;
__IO uint32_t ADC_Voltage = 0;

/////////////////////////////////////////////////////////////////////////////////
/* 		Configuracion del puerto Serial          USART_1               */
/////////////////////////////////////////////////////////////////////////////////

//#define	DMA


void serialPC(){
 
__enable_irq();

  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
   
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  // RCC_APB1PeriphClockCmd
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  //USARTx_CLK_INIT(USART1_CLK, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  // -----> Tx
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // -----> Rx


  /* Configure USART Tx as alternate function  */

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follows:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - Two Stop Bit
        - Odd parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);



   // Configuracion de las interrupciones

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	NVIC_EnableIRQ(USART1_IRQn);

   USART_Cmd(USART1, ENABLE);
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

/////////////////////////////////////////////////////////////////////////////////
/* 		Configuracion del conversor ADC                     */
/////////////////////////////////////////////////////////////////////////////////

void config_ADC()
{
   ADC_InitTypeDef 		ADC_InitStructure;
   ADC_CommonInitTypeDef 	ADC_CommonInitStructure;
   DMA_InitTypeDef 		DMA_InitStructure;
   GPIO_InitTypeDef		GPIO_InitStructure;  
   
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
   //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  
   ADC_DeInit();
   
#ifdef DMA
  
   DMA_InitStructure.DMA_Channel = DMA_Channel_2;
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_ADDRESS;
   DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_Value;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   
   DMA_InitStructure.DMA_BufferSize = 1;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMA_Init(DMA2_Stream0, &DMA_InitStructure);
   DMA_Cmd(DMA2_Stream0, ENABLE);
   
#endif
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA, &GPIO_InitStructure);


   ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
   ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
   ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
   ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
   ADC_CommonInit(&ADC_CommonInitStructure);
   

 
 // Configure ADC1 Channel 7  PA7
 
   ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
   ADC_InitStructure.ADC_ScanConvMode = ENABLE;   
   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
#ifdef DMA
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
#else
   ADC_InitStructure.ADC_ExternalTrigConv = DISABLE;
#endif
   ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfConversion = 2;
   ADC_Init(ADC1, &ADC_InitStructure);
   //ADC_Init(ADC2, &ADC_InitStructure);
   
   ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
   ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_3Cycles);

#ifdef DMA   
   ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);   
   ADC_DMACmd(ADC1,ENABLE);
#endif 
   
   ADC_Cmd(ADC1,ENABLE);
   //ADC_Cmd(ADC2,ENABLE);
     
}

uint32_t readData_ADC1(){

#ifdef DMA  
  ADC_Voltage = ADC_Value*3300/0xFFF;
  return ADC_Voltage;
#else
     ADC_SoftwareStartConv(ADC1);
     while(ADC_GetSoftwareStartConvStatus(ADC1) != RESET){};
     uint16_t value1 = (uint16_t)(ADC_GetConversionValue(ADC1)*3300/0xFFF);
     ADC_SoftwareStartConv(ADC1);
     uint16_t value2 = (uint16_t)(ADC_GetConversionValue(ADC1)*3300/0xFFF);
     
     sprintf((char*)trasmitDataS,"VALUE1 %d VALUE2 %d\r\n",value1, value2);
     sendData((char*)trasmitDataS);
#endif
  return 0;
  
}

uint32_t readData_ADC2(){
    ADC_SoftwareStartConv(ADC1);
     while(ADC_GetSoftwareStartConvStatus(ADC1) != RESET){};
     return (uint32_t)(ADC_GetConversionValue(ADC1)*3300/0xFFF);
}

