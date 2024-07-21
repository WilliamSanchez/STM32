/*
	USART_1 PC		I2C_1 BMP280 

	Tx ==> PA9		SCL ==> PB8
	Rx ==> PA10		SDA ==> PB9
*/



#include <stm32f30x.h>
#include <string.h>
#include <stdio.h>

#include "modulo3G.h"
#include "configI2C.h"
#include "misc.h"

  #define LCDHEIGHT		    32
  #define SSD1306_LCDWIDTH          128
  #define CHARS_COLS_LENGTH  	    5

  // Success
  // -------------------------------------------
  #define SSD1306_SUCCESS           0

  // Error
  // -------------------------------------------
  #define SSD1306_ERROR             1

  // Address definition
  // -----------------------------------
  #define SSD1306_ADDRESS           0x3C

  // Command definition
  // -----------------------------------
  #define SSD1306_COMMAND           0x80  // Continuation bit=1, D/C=0; 1000 0000
  #define SSD1306_COMMAND_STREAM    0x00  // Continuation bit=0, D/C=0; 0000 0000
  #define SSD1306_DATA              0xC0  // Continuation bit=1, D/C=1; 1100 0000
  #define SSD1306_DATA_STREAM       0x40  // Continuation bit=0, D/C=1; 0100 0000

  #define SSD1306_SET_MUX_RATIO     0xA8
  #define SSD1306_DISPLAY_OFFSET    0xD3
  #define SSD1306_DISPLAY_ON        0xAF
  #define SSD1306_DISPLAY_OFF       0xAE
  #define SSD1306_DIS_ENT_DISP_ON   0xA4
  #define SSD1306_DIS_IGNORE_RAM    0xA5
  #define SSD1306_DIS_NORMAL        0xA6
  #define SSD1306_DIS_INVERSE       0xA7
  #define SSD1306_DEACT_SCROLL      0x2E
  #define SSD1306_ACTIVE_SCROLL     0x2F
  #define SSD1306_SET_START_LINE    0x40
  #define SSD1306_MEMORY_ADDR_MODE  0x20
  #define SSD1306_SET_COLUMN_ADDR   0x21
  #define SSD1306_SET_PAGE_ADDR     0x22
  #define SSD1306_SEG_REMAP         0xA0
  #define SSD1306_SEG_REMAP_OP      0xA1
  #define SSD1306_COM_SCAN_DIR      0xC0
  #define SSD1306_COM_SCAN_DIR_OP   0xC8
  #define SSD1306_COM_PIN_CONF      0xDA
  #define SSD1306_SET_CONTRAST      0x81
  #define SSD1306_SET_OSC_FREQ      0xD5
  #define SSD1306_SET_CHAR_REG      0x8D
  #define SSD1306_SET_PRECHARGE     0xD9
  #define SSD1306_VCOM_DESELECT     0xDB

  // AREA definition
  // -----------------------------------
  #define START_PAGE_ADDR           0
  #define END_PAGE_ADDR             3
  #define START_COLUMN_ADDR         0
  #define END_COLUMN_ADDR           127

  #define CACHE_SIZE_MEM            LCDHEIGHT*SSD1306_LCDWIDTH/8 

  #define MAX_X                     END_COLUMN_ADDR
  #define MAX_Y                     (END_PAGE_ADDR+1)*8


char trasmitData[64];

static uint8_t buffer[CACHE_SIZE_MEM];


uint8_t display();
void drawLetter();
void drawChar();
void drawChar1();
void sendData();
void clearDisplay();

volatile int i = 0;

/** @array Charset */
const uint8_t FONTS[][CHARS_COLS_LENGTH] PROGMEM = {
  { 0x00, 0x00, 0x00, 0x00, 0x00 }, //0 20 space
  { 0x81, 0x81, 0x18, 0x81, 0x81 }, //1 21 !
  { 0x00, 0x07, 0x00, 0x07, 0x00 }, //2 22 "
  { 0x14, 0x7f, 0x14, 0x7f, 0x14 }, //3 23 #
  { 0x24, 0x2a, 0x7f, 0x2a, 0x12 }, //4 24 $
  { 0x23, 0x13, 0x08, 0x64, 0x62 }, //5 25 %
  { 0x36, 0x49, 0x55, 0x22, 0x50 }, //6 26 &
  { 0x00, 0x05, 0x03, 0x00, 0x00 }, //7 27 '
  { 0x00, 0x1c, 0x22, 0x41, 0x00 }, //8 28 (
  { 0x00, 0x41, 0x22, 0x1c, 0x00 }, //9 29 )
  { 0x14, 0x08, 0x3e, 0x08, 0x14 }, //10 2a *
  { 0x08, 0x08, 0x3e, 0x08, 0x08 }, //11 2b +
  { 0x00, 0x50, 0x30, 0x00, 0x00 }, //12 2c ,
  { 0x08, 0x08, 0x08, 0x08, 0x08 }, //13 2d -
  { 0x00, 0x60, 0x60, 0x00, 0x00 }, //14 2e .
  { 0x20, 0x10, 0x08, 0x04, 0x02 }, //15 2f /
  { 0x3e, 0x51, 0x49, 0x45, 0x3e }, //16 30 0
  { 0x00, 0x42, 0x7f, 0x40, 0x00 }, //17 31 1
  { 0x42, 0x61, 0x51, 0x49, 0x46 }, //18 32 2
  { 0x21, 0x41, 0x45, 0x4b, 0x31 }, //19 33 3
  { 0x18, 0x14, 0x12, 0x7f, 0x10 }, //20 34 4
  { 0x27, 0x45, 0x45, 0x45, 0x39 }, //21 35 5
  { 0x3c, 0x4a, 0x49, 0x49, 0x30 }, //22 36 6
  { 0x01, 0x71, 0x09, 0x05, 0x03 }, //23 37 7
  { 0x36, 0x49, 0x49, 0x49, 0x36 }, //24 38 8
  { 0x06, 0x49, 0x49, 0x29, 0x1e }, //25 39 9
  { 0x00, 0x36, 0x36, 0x00, 0x00 }, //26 3a :
  { 0x00, 0x56, 0x36, 0x00, 0x00 }, //27 3b ;
  { 0x08, 0x14, 0x22, 0x41, 0x00 }, //28 3c <
  { 0x14, 0x14, 0x14, 0x14, 0x14 }, //29 3d =
  { 0x00, 0x41, 0x22, 0x14, 0x08 }, //30 3e >
  { 0x02, 0x01, 0x51, 0x09, 0x06 }, //31 3f ?
  { 0x32, 0x49, 0x79, 0x41, 0x3e }, //32 40 @
  { 0x7e, 0x11, 0x11, 0x11, 0x7e }, //33 41 A
  { 0x7f, 0x49, 0x49, 0x49, 0x36 }, //34 42 B
  { 0x3e, 0x41, 0x41, 0x41, 0x22 }, //35 43 C
  { 0x7f, 0x41, 0x41, 0x22, 0x1c }, //36 44 D
  { 0x7f, 0x49, 0x49, 0x49, 0x41 }, //37 45 E
  { 0x7f, 0x09, 0x09, 0x09, 0x01 }, //38 46 F
  { 0x3e, 0x41, 0x49, 0x49, 0x7a }, //39 47 G
  { 0x7f, 0x08, 0x08, 0x08, 0x7f }, //40 48 H
  { 0x00, 0x41, 0x7f, 0x41, 0x00 }, //41 49 I
  { 0x20, 0x40, 0x41, 0x3f, 0x01 }, //42 4a J
  { 0x7f, 0x08, 0x14, 0x22, 0x41 }, //43 4b K
  { 0x7f, 0x40, 0x40, 0x40, 0x40 }, //44 4c L
  { 0x7f, 0x02, 0x0c, 0x02, 0x7f }, //45 4d M
  { 0x7f, 0x04, 0x08, 0x10, 0x7f }, //46 4e N
  { 0x3e, 0x41, 0x41, 0x41, 0x3e }, //47 4f O
  { 0x7f, 0x09, 0x09, 0x09, 0x06 }, //48 50 P
  { 0x3e, 0x41, 0x51, 0x21, 0x5e }, //49 51 Q
  { 0x7f, 0x09, 0x19, 0x29, 0x46 }, //50 52 R
  { 0x46, 0x49, 0x49, 0x49, 0x31 }, //51 53 S
  { 0x01, 0x01, 0x7f, 0x01, 0x01 }, //52 54 T
  { 0x3f, 0x40, 0x40, 0x40, 0x3f }, //53 55 U
  { 0x1f, 0x20, 0x40, 0x20, 0x1f }, //54 56 V
  { 0x3f, 0x40, 0x38, 0x40, 0x3f }, //55 57 W
  { 0x63, 0x14, 0x08, 0x14, 0x63 }, //56 58 X
  { 0x07, 0x08, 0x70, 0x08, 0x07 }, //57 59 Y
  { 0x61, 0x51, 0x49, 0x45, 0x43 }, //58 5a Z
  { 0x00, 0x7f, 0x41, 0x41, 0x00 }, //59 5b [
  { 0x02, 0x04, 0x08, 0x10, 0x20 }, //60 5c backslash
  { 0x00, 0x41, 0x41, 0x7f, 0x00 }, //61 5d ]
  { 0x04, 0x02, 0x01, 0x02, 0x04 }, //62 5e ^
  { 0x40, 0x40, 0x40, 0x40, 0x40 }, //63 5f _
  { 0x00, 0x01, 0x02, 0x04, 0x00 }, //63 60 `
  { 0x20, 0x54, 0x54, 0x54, 0x78 }, //65 61 a
  { 0x7f, 0x48, 0x44, 0x44, 0x38 }, //66 62 b
  { 0x38, 0x44, 0x44, 0x44, 0x20 }, //67 63 c
  { 0x38, 0x44, 0x44, 0x48, 0x7f }, //68 64 d
  { 0x38, 0x54, 0x54, 0x54, 0x18 }, //69 65 e
  { 0x08, 0x7e, 0x09, 0x01, 0x02 }, //70 66 f
  { 0x0c, 0x52, 0x52, 0x52, 0x3e }, //71 67 g
  { 0x7f, 0x08, 0x04, 0x04, 0x78 }, //72 68 h
  { 0x00, 0x44, 0x7d, 0x40, 0x00 }, //73 69 i
  { 0x20, 0x40, 0x44, 0x3d, 0x00 }, //74 6a j
  { 0x7f, 0x10, 0x28, 0x44, 0x00 }, //75 6b k
  { 0x00, 0x41, 0x7f, 0x40, 0x00 }, //76 6c l
  { 0x7c, 0x04, 0x18, 0x04, 0x78 }, //77 6d m
  { 0x7c, 0x08, 0x04, 0x04, 0x78 }, //78 6e n
  { 0x38, 0x44, 0x44, 0x44, 0x38 }, //79 6f o
  { 0x7c, 0x14, 0x14, 0x14, 0x08 }, //80 70 p
  { 0x08, 0x14, 0x14, 0x14, 0x7c }, //81 71 q
  { 0x7c, 0x08, 0x04, 0x04, 0x08 }, //82 72 r
  { 0x48, 0x54, 0x54, 0x54, 0x20 }, //83 73 s
  { 0x04, 0x3f, 0x44, 0x40, 0x20 }, //84 74 t
  { 0x3c, 0x40, 0x40, 0x20, 0x7c }, //85 75 u
  { 0x1c, 0x20, 0x40, 0x20, 0x1c }, //86 76 v
  { 0x3c, 0x40, 0x30, 0x40, 0x3c }, //87 77 w
  { 0x44, 0x28, 0x10, 0x28, 0x44 }, //88 78 x
  { 0x0c, 0x50, 0x50, 0x50, 0x3c }, //89 79 y
  { 0x44, 0x64, 0x54, 0x4c, 0x44 }, //90 7a z
  { 0x00, 0x08, 0x36, 0x41, 0x00 }, //91 7b {
  { 0x00, 0x00, 0x7f, 0x00, 0x00 }, //92 7c |
  { 0x00, 0x41, 0x36, 0x08, 0x00 }, //93 7d }
  { 0x10, 0x08, 0x08, 0x10, 0x08 }, //94 7e ~
  { 0x00, 0x00, 0x00, 0x00, 0x00 }  //95 7f
};

int main(void){

    InitializeLEDs();
    delay_init();
    Init_I2C1();
    configUSART_1();
    configButton();
    uint8_t data;
    
    I2C_WriteReg(0xAE);
    I2C_WriteReg(0xD5);
    I2C_WriteReg(0x80);

    I2C_WriteReg(0xA8);
    I2C_WriteReg(LCDHEIGHT-1);

    I2C_WriteReg(0xD3);
    I2C_WriteReg(0x0);

    I2C_WriteReg(0x40|0x0);

    I2C_WriteReg(0x8D);
    I2C_WriteReg(0x14);// I2C_WriteReg(0x10);//
     	

    I2C_WriteReg(0x20);
    I2C_WriteReg(0x00);

    I2C_WriteReg(0xA0 | 0x1);
    I2C_WriteReg(0xC8);

    I2C_WriteReg(0xDA);
    I2C_WriteReg(0x02);
    I2C_WriteReg(0x81);
    I2C_WriteReg(0x8F);

    I2C_WriteReg(0xD9);
    I2C_WriteReg(0xF1);// I2C_WriteReg(0x22);// I2C_WriteReg(0xF1);//

    I2C_WriteReg(0xDB);
    I2C_WriteReg(0x40);

    I2C_WriteReg(0xA4);
    I2C_WriteReg(0xA6);
    I2C_WriteReg(0x2E);
    I2C_WriteReg(0xAF);

    delay(1000);display();
    delay(1000);clearDisplay();

    while(1)
    {  	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
        delay(1000);
	drawLetter();display();  //2
        GPIO_ResetBits(GPIOE,GPIO_Pin_11);
        delay(1000); clearDisplay();
	GPIO_ResetBits(GPIOE,GPIO_Pin_10);
	drawChar(); display(); //1
        GPIO_ResetBits(GPIOE,GPIO_Pin_11);
        delay(1000);clearDisplay();
    }

  return 0;
}

void sendData(){
        //sprintf(trasmitData,"AT [%x] ==> (%d)\r",I2C_WriteReg(0xDA),i++);
       	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
}

void clearDisplay(){
    memset(buffer,0x00,CACHE_SIZE_MEM);
}

uint8_t display(){

  uint8_t da = 0;

    I2C_WriteReg(0x21);
    I2C_WriteReg(0);
    I2C_WriteReg(127);

    I2C_WriteReg(0x22);
    I2C_WriteReg(0);
    I2C_WriteReg(END_PAGE_ADDR);

   uint8_t numDatos = 128;
    for(uint16_t i=0; i<CACHE_SIZE_MEM/numDatos;i++){
      da =  I2C_WriteBuffer(buffer+i*numDatos,(uint8_t*)(&numDatos));
    }

    return da;

}

void drawChar1(){

  for(int i=0; i<CACHE_SIZE_MEM; i++){
       buffer[i]=0xFF;
  }

}


void drawLetter(){

  // William

  for(int i=0; i<5; i++){
       buffer[i]=FONTS[55][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+8]=FONTS[73][i];
  }
  for(int i=0; i<5; i++){
       buffer[i+8*2]=FONTS[76][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+8*3]=FONTS[76][i];
  }
  for(int i=0; i<5; i++){
       buffer[i+8*4]=FONTS[73][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+8*5]=FONTS[65][i];
  }
  for(int i=0; i<5; i++){
       buffer[i+8*6]=FONTS[77][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+8*7]=FONTS[14][i];
  }

  // Sanchez

  for(int i=0; i<5; i++){
       buffer[i+128]=FONTS[51][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+128+8]=FONTS[65][i];
  }
  for(int i=0; i<5; i++){
       buffer[i+128+8*2]=FONTS[78][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+128+8*3]=FONTS[67][i];
  }
  for(int i=0; i<5; i++){
       buffer[i+128+8*4]=FONTS[72][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+128+8*5]=FONTS[69][i];
  }
  for(int i=0; i<5; i++){
       buffer[i+128+8*6]=FONTS[90][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+128+8*7]=FONTS[14][i];
  }

  // Farfan

  for(int i=0; i<5; i++){
       buffer[i+128*2]=FONTS[38][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+128*2+8]=FONTS[65][i];
  }
  for(int i=0; i<5; i++){
       buffer[i+128*2+8*2]=FONTS[82][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+128*2+8*3]=FONTS[70][i];
  }
  for(int i=0; i<5; i++){
       buffer[i+128*2+8*4]=FONTS[65][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+128*2+8*5]=FONTS[78][i];
  }

  for(int i=0; i<5; i++){
       buffer[i+128*2+8*6]=FONTS[14][i];
  }

  for(int i=0; i<128; i++){
       buffer[i+128*3]=0xF0;
  }

}

void drawChar(){

  for(int i=0; i<CACHE_SIZE_MEM; i++){
       buffer[i]=0xAA;
  }

}


