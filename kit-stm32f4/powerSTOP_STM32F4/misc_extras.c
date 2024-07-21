#include<misc_extras.h>

GPIO_InitTypeDef 	GPIO_InitStructure;
static u8  fac_us=0;  // micro second count
static u16 fac_ms=0;  // mili second countS

EXTI_InitTypeDef	EXTI_InitStructure;
NVIC_InitTypeDef 	NVIC_InitStructure;


char txBuffer_1[1024]="";

RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;
RTC_InitTypeDef RTC_InitStructure;

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
char buffer[80];
uint32_t subsec = 0;

void InitializeLEDs(){	

	GPIO_InitTypeDef gpioStructure;

//
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Disable GPIOs clock //
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, DISABLE);
////


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

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

void configButton(){
/*
  // Enable GPIOC clock //
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  // Enable SYSCFG clock //
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  // Connect EXTI5 Line to PA6 pin //
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
  
  // Configure EXTI6 line //
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  // Enable and set EXTI4_15 Interrupt //
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/
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

  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div16, Wakeup Time Base: ~4s

     Wakeup Time Base = (16 / (LSE or LSI)) * WakeUpCounter
  */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(0x1FFF);

  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);

  /* Connect EXTI_Line22 to the RTC Wakeup event */
  EXTI_ClearITPendingBit(EXTI_Line22);
  EXTI_InitStructure.EXTI_Line = EXTI_Line22;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

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

   RTC_TimeStructure.RTC_Hours = hours;//ano;

   RTC_TimeStructure.RTC_Minutes = minutes;//ano;
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



//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


void SysTick_Configuration(void)
{
  /* SysTick interrupt each 250 ms */
  //   if (SysTick_Config((SystemCoreClock/8) / 4))
  if (SysTick_Config((SystemCoreClock/2) / 4))
  { 
    /* Capture error */ 
    while (1);
  }

  /* Select AHB clock(HCLK) divided by 8 as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  /* Set SysTick Preemption Priority to 1 */
  NVIC_SetPriority(SysTick_IRQn, 0x04);
}


///////////////////////////////////////////////////////////


void SYSCLKConfig_STOP(void)
{
  /* After wake-up from STOP reconfigure the system clock */
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* Wait till HSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
  }
  
  /* Enable PLL */
  RCC_PLLCmd(ENABLE);
  
  /* Wait till PLL is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {
  }
  
  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x08)
  {
  }
}

