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
	gpioStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_13;
	gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioStructure);
	GPIO_WriteBit(GPIOB, GPIO_Pin_0 | GPIO_Pin_1, Bit_SET);

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
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStructure); 
}


void configButton_mode() {

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

void configButton_delay() {

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

/////////////////////////////////////////////////////////////////////////////////
/* 		Configuracion del puerto Serial          USART_1               */
/////////////////////////////////////////////////////////////////////////////////

__IO uint16_t ADC_Value = 0;
__IO uint32_t ADC_Voltage = 0;

//#define	DMA

void config_ADC()
{
   ADC_InitTypeDef 		ADC_InitStructure;
   ADC_CommonInitTypeDef 	ADC_CommonInitStructure;
   DMA_InitTypeDef 		DMA_InitStructure;
   GPIO_InitTypeDef		GPIO_InitStructure;  
   
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  
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
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

#ifdef DMA

   ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
   ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
   ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
   ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
   ADC_CommonInit(&ADC_CommonInitStructure);
   
#endif
 
 // Configure ADC1 Channel 7  PA7
 
   ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
   ADC_InitStructure.ADC_ScanConvMode = DISABLE;   
   ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
#ifdef DMA
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
#else
   ADC_InitStructure.ADC_ExternalTrigConv = DISABLE;
#endif
   ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfConversion = 1;
   ADC_Init(ADC1, &ADC_InitStructure);
   ADC_Init(ADC2, &ADC_InitStructure);
   
   ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_3Cycles);
   //ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 2, ADC_SampleTime_3Cycles);

#ifdef DMA   
   ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);   
   ADC_DMACmd(ADC1,ENABLE);
#endif 
   
   ADC_Cmd(ADC1,ENABLE);
   ADC_Cmd(ADC2,ENABLE);
  
}

uint32_t readData_ADC1(){

#ifdef DMA  
  ADC_Voltage = ADC_Value*3300/0xFFF;
  return ADC_Voltage;
#else
     while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
     return (uint32_t)(ADC_GetConversionValue(ADC1)*3300/0xFFF);
#endif
  
}

uint32_t readData_ADC2(){

     //while (!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));
     //return (uint32_t)(ADC_GetConversionValue(ADC2)*3300/0xFFF);
     return 0;
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