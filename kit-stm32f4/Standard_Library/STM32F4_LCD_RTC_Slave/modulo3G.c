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
  USART_InitStructure.USART_BaudRate = 9600;
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
/* 		Configuracion del puerto Serial   USART 2                      */
/////////////////////////////////////////////////////////////////////////////////

void configUSART_2(){

    USART_InitTypeDef	USART_InitStructure;
    NVIC_InitTypeDef 	NVIC_InitStructure;

  /* enable usart clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
    // Configuracion de los puertos de entrada

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO_PuPd_UP;
  
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // -----> Tx
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  // -----> Rx

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


/////////////////////////////////////////////////////////
///////////   RTC Configuration            ///////////////
/////////////////////////////////////////////////////////


void Config_RTC(){

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

   PWR_BackupAccessCmd(ENABLE);
  
   RCC_BackupResetCmd(ENABLE);
   RCC_BackupResetCmd(DISABLE);

  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

   RCC_RTCCLKCmd(ENABLE);
   RTC_WaitForSynchro();
}

//////////////////  Init RTC

void Init_RTC(uint8_t hours, uint8_t minutes, uint8_t seconds){

   Config_RTC();
   //Config_Alarm();

   /* Configure the RTC data register and RTC prescaler */
   RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
   RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
   RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
   RTC_Init(&RTC_InitStructure);   

 //  sprintf(buffer,"\n\r  Configuracion del reloj de tiempo real : ");
//   USARTSendStr(buffer);

//   sprintf(buffer,"\n\r input the hour :   ");
//   USARTSendStr(buffer);
//   ano = USARTRecvStr();
   RTC_TimeStructure.RTC_Hours = hours;//ano;

//   sprintf(buffer,"\n\r input the minutes :   ");
//   USARTSendStr(buffer);
//   ano = USARTRecvStr();
   RTC_TimeStructure.RTC_Minutes = minutes;//ano;


//   sprintf(buffer,"\n\r input the seconds :   ");
//   USARTSendStr(buffer);
//   ano = USARTRecvStr();
   RTC_TimeStructure.RTC_Seconds = seconds;//ano;


   RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

   RTC_TimeStructInit(&RTC_TimeStructure);
   RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);

   RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
   
}

void set_time(time *time_ptr, uint8_t hora, uint8_t minutos, uint8_t segundos)
{


   RTC_TimeStructure.RTC_Hours = hora;//hora;
   RTC_TimeStructure.RTC_Minutes = minutos;//minuto;
   RTC_TimeStructure.RTC_Seconds = segundos;//segundos;
   RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

   time_ptr->hours=hora;
   time_ptr->minutes=minutos;
   time_ptr->seconds=segundos;

}


void begin_RTC(uint8_t hours, uint8_t minutes, uint8_t seconds){
     Init_RTC(hours,minutes,seconds);
     set_time(&timeData, hours, minutes, seconds);
}


uint8_t getHoras(){
   RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
   return RTC_TimeStructure.RTC_Hours;
}

uint8_t getMinutos(){
   RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
   return RTC_TimeStructure.RTC_Minutes;

}

uint8_t getSegundos(){
   RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
   return RTC_TimeStructure.RTC_Seconds;
}

uint8_t getmillSegundos(){

  subsec = 1000 - ((uint32_t)RTC_GetSubSecond() * 1000 )/ 256;
  return (uint8_t)(subsec/100)*10+(uint8_t)(uint8_t)((subsec%100)/10); 
}



