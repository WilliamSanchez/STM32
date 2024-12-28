#include<misc_extras.h>

GPIO_InitTypeDef 	GPIO_InitStructure;
static u8  fac_us=0;  // micro second count
static u16 fac_ms=0;  // mili second countS

EXTI_InitTypeDef	EXTI_InitStructure;
NVIC_InitTypeDef 	NVIC_InitStructure;

time timeData;

char txBuffer_1[1024]="";

RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;
RTC_InitTypeDef RTC_InitStructure;

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
char buffer[80];
uint32_t subsec = 0;

void InitializeLEDs(){	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef gpioStructure;
	
	gpioStructure.GPIO_Pin = GPIO_Pin_13;
	gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioStructure);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);


}

void delay_init(void){
   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
   fac_us=SystemCoreClock/8000000;							//SYSCLK/8  
   fac_ms=(u16)fac_us*1000;
}

void delay(__IO uint32_t nTime){ 

	u32 temp;	    	 
	SysTick->LOAD=nTime*fac_ms; //Load	  		 
	SysTick->VAL=0x00;        		//Clear
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;        
	do{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       
	SysTick->VAL =0X00; 
}

//////////////   delay Us

void delay_us(u32 micro_sec)
{	
//	delay_init();	
	u32 temp;	    	 
	SysTick->LOAD=micro_sec*fac_us; //Load	  		 
	SysTick->VAL=0x00;        		//Clear
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;        
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       
	SysTick->VAL =0X00;       
}

void configButton_Rudder(){
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

/* Enable the BUTTON Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure Button pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO_PuPd_DOWN; //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Connect EXTI5 Line to PD5 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  
  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStructure); 
}


void configButton_ThP() {

  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

/* Enable the BUTTON Clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure Button pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP; //GPIO_PuPd_DOWN; //GPIO_PuPd_NOPULL; //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Connect EXTI5 Line to PD5 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);

  
  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0A;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStructure); 

}

void configButton_ThM() {

  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

/* Enable the BUTTON Clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure Button pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP; //GPIO_PuPd_DOWN; //GPIO_PuPd_NOPULL; //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Connect EXTI5 Line to PD5 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);

  
  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0A;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStructure); 

}

uint8_t parseData(const char *data1, uint8_t len1, const char *data2){
  
  uint8_t i = 0, j = 0; 

  for(i = 0; i<len1; i++){
     if(*(data1+i) == *(data2+j)){
       j++;
       if(*(data2+j) == '\0') return 1; //
     }else {
       j = 0;
     }    
   }
	
  return 0;
}

void sendData(const char *data){

     sprintf(txBuffer_1,"%s",data);
     USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
     while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
  
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

uint32_t getmillSegundos(){
  //subsec = 1000 - ((uint32_t)RTC_GetSubSecond() * 1000 )/ 256;
  subsec = 1000 - ((uint32_t)((uint32_t)RTC_GetSubSecond() * 1000) / (uint32_t)0x3FF);
  return subsec; //(uint16_t)(subsec/100)*10+(uint16_t)((subsec%100)/10); 
}
