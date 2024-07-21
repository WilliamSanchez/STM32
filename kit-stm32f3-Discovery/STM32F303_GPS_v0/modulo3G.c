#include <modulo3G.h>


RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;
RTC_InitTypeDef RTC_InitStructure;

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
char buffer[80];
uint32_t subsec = 0;

/////////////////////////////////////////////////////////////////////////////////
/* 		Configuracion del puerto Serial          USART_1               */
/////////////////////////////////////////////////////////////////////////////////

void configUSART_1(){

    USART_InitTypeDef	USART_InitStructure;
    NVIC_InitTypeDef 	NVIC_InitStructure;

  // enable usart clock //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
   
    // Configuracion de los puertos de entrada

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO_PuPd_UP;
  
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);  // -----> Tx
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);  // -----> Rx

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configuracion del puerto serial  

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);

   // Configuracion de las interrupciones

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

}

/////////////////////////////////////////////////////////////////////////////////
/* 		Configuracion del puerto Serial   USART 2                      */
/////////////////////////////////////////////////////////////////////////////////

void configUSART_2(){

    USART_InitTypeDef	USART_InitStructure;
    NVIC_InitTypeDef 	NVIC_InitStructure;

  // enable usart clock //
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
    // Configuracion de los puertos de entrada

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO_PuPd_UP;
  
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);  // -----> Tx
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);  // -----> Rx

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configuracion del puerto serial  

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);

   // Configuracion de las interrupciones
 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

}

void configUSART_3(){

    USART_InitTypeDef	USART_InitStructure;
    NVIC_InitTypeDef 	NVIC_InitStructure;

  /* enable usart clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
   
    // Configuracion de los puertos de entrada

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // GPIO_PuPd_NOPULL; 
  
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_7);  // -----> Tx
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_7);  // -----> Rx

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Configuracion del puerto serial  

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART3, &USART_InitStructure);

   // Configuracion de las interrupciones

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART3, ENABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

}
 




