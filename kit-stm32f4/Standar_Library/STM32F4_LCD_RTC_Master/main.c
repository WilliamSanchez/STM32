/*
	USART_1 PC		I2C_1 BMP280 

	Tx ==> PA9		SCL ==> PB8
	Rx ==> PA10		SDA ==> PB9
*/

#include <stm32f4xx.h>
#include <string.h>
#include <stdio.h>

#include <modulo3G.h>
#include "configI2C.h"
#include "misc_extras.h"
#include "lcdOLED.h"



volatile int i = 0;
int numero = 0;

uint8_t trasmitData[64];
uint8_t trasmitDataS[64];
char txBuffer_1[255]="";



int main(void){

    USART_DeInit(USART1);
    Init_I2C1();
    delay_init();
    configUSART_1();
    InitializeLEDs();
    configButton();

    begin_RTC(00, 00, 00);

    LCD_Init();

    delay(1000);display();
    delay(1000);clearDisplay();

    sprintf(txBuffer_1,"--> Time: %02d:%02d:%02d <--\n\r", getHoras(), getMinutos(), getSegundos());
    sendData();

    while(1)
    {  	
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        GPIO_ResetBits(GPIOB,GPIO_Pin_0);
        //sprintf(txBuffer_1,"AT+CMGS=\"+%d\"\r",numero++); sendData();
        delay(10);GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        uint8_t hora = getHoras(); uint8_t minuto=getMinutos();
        uint8_t segundo = getSegundos(); uint8_t miliseg = getmillSegundos();
        sprintf((char*)trasmitData,"%02d:%02d:%02d:%02d ", hora, minuto, segundo, miliseg);
        sprintf((char*)trasmitDataS,"%d.%2d s",(hora*3600+minuto*60+segundo),miliseg);
	drawLetter((uint8_t*)"MASTER",(uint8_t*)" ",trasmitData,trasmitDataS);display();  //2
        delay(10); clearDisplay();
    }

  return 0;
}



void sendData(){
        //sprintf(trasmitData,"AT [%x] ==> (%d)\r",I2C_WriteReg(0xDA),i++);
       	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
}



