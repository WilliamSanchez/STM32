#include<misc.h>

GPIO_InitTypeDef 	GPIO_InitStructure;
static u8  fac_us=0;  // micro second count
static u16 fac_ms=0;  // mili second countS

EXTI_InitTypeDef	EXTI_InitStructure;
NVIC_InitTypeDef 	NVIC_InitStructure;

void InitializeLEDs(){	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &gpioStructure);
	GPIO_WriteBit(GPIOE, GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12, Bit_RESET);
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

void configButton(){

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

}


uint8_t parseData(const char *data1, uint8_t len1, const char *data2){
  
  uint8_t i = 0, j = 0; 
 
   if(len1 <= 0 )
	return 1;

  for(i = 0; i<len1; i++){
     if(*(data1+i) == *(data2+j)){
       j++;
       if(*(data2+j) == '\0') return 1; 
     }else {
       j = 0;
     }    
   }
	
  return 0;
}
