/*
	USART_1 PC		USART_2 BLE

	Tx ==> PA9		Tx ==> PA2
	Rx ==> PA10		Rx ==> PA3
*/

#include <stm32f30x.h>
#include <string.h>
#include <stdio.h>

#include "modulo3G.h"
#include "misc.h"

#define la(x)	'S' ? -1:1
#define lo(x)	'W' ? -1:1

char trasmitData[1024];
uint8_t memoria1[1024];
uint8_t memoria2[1024];

uint8_t dataGPS = 0;

volatile int i = 0;
float getLatitude(float latitude, char la);
float getLongitude(float longitude, char lo);

int main(void){

    InitializeLEDs();
    delay_init();
    configUSART_1();
    configUSART_2();  
    configButton();

    uint16_t numero = 0;
    delay(1000);
    while(1)
    {  	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
        delay(1000);
        if(dataGPS == 1){
            float latitude;
            float longitude;
            char la,lo;
            if(sscanf((char*)trasmitData,"%f %c %f %c",&latitude,&la,&longitude,&lo)){
	       sprintf((char*)memoria1,"\n\r[%d] latitude: %f longitude: %f",numero++,getLatitude(latitude,la),getLongitude(longitude,lo));
 	      USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
              while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
            }

            memset(trasmitData,0x00,strlen((char*)trasmitData));
            memset(memoria2,0x00,strlen((char*)memoria2));
            dataGPS = 0;
        }
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        delay(1000);
    }

  return 0;
}

float getLatitude(const float latitude, char la){

 static float entlatitude = 0;
 static float declatitude = 0; 
 entlatitude = (int)latitude/100;
 declatitude = (latitude-entlatitude*100)/60; 
 entlatitude += declatitude;
 return entlatitude *= la(la);

}


float getLongitude(const float longitude,char lo){

 static float entlongitude = 0;
 static float declongitude = 0; 
 entlongitude = (int)longitude/100;
 declongitude = (longitude-entlongitude*100)/60; 
 entlongitude += declongitude;
 return entlongitude *= lo(lo);

}



