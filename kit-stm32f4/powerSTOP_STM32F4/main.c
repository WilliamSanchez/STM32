/*
	USART_1 PC		 

	Tx ==> PA9
	Rx ==> PA10

*/

#include <stm32f4xx.h>
#include <stdio.h>

#include "misc_extras.h"


#define max(x,y)    (x>y) ? x:y



int main(void){

    InitializeLEDs();  
    delay_init();    
    SysTick_Configuration();
    begin_RTC(00, 00, 00);
 
    while(1)
    {
      GPIO_SetBits(GPIOC,GPIO_Pin_13);
      delay(5000);
   
      /* Enable Wakeup Counter */
      RTC_WakeUpCmd(ENABLE);

      /* Enter Stop Mode */
      PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    
      /* Disable Wakeup Counter */
    RTC_WakeUpCmd(DISABLE);

      /* Configures system clock after wake-up from STOP: enable HSE, PLL and select 
       PLL as system clock source (HSE and PLL are disabled in STOP mode) */
      SYSCLKConfig_STOP();
      
      //GPIO_ResetBits(GPIOC,GPIO_Pin_13);
      //delay(5000);
    }
  return 0;
}

