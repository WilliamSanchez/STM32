#include "stm32f4xx.h"
#include "lcdOLED.h"

#include <string.h>

#include "fonts.h"

static uint8_t buffer[CACHE_SIZE_MEM];

void LCD_Init(){

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
//    I2C_WriteReg(0x02);   	// 32 height
    I2C_WriteReg(0x12);		// 64 height 
//    I2C_WriteReg(0x10);		// 16 height 
    I2C_WriteReg(0x81);		
//    I2C_WriteReg(0x8F);		//32 height
    I2C_WriteReg(0x9F);		// 64 height
//    I2C_WriteReg(0xAF);		// 16 height

    I2C_WriteReg(0xD9);
    I2C_WriteReg(0xF1);// I2C_WriteReg(0x22);// I2C_WriteReg(0xF1);//

    I2C_WriteReg(0xDB);
    I2C_WriteReg(0x40);

    I2C_WriteReg(0xA4);
    I2C_WriteReg(0xA6);
    I2C_WriteReg(0x2E);
    I2C_WriteReg(0xAF);

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

   uint8_t numDatos = 129;
    for(uint16_t i=0; i<(CACHE_SIZE_MEM)/numDatos;i++){
      da =  I2C_WriteBuffer(buffer+i*numDatos,(uint8_t*)(&numDatos));
    }

    return da;

}


void drawChar1(){

  for(int i=0; i<CACHE_SIZE_MEM; i++){
       buffer[i]=0xFF;
  }

}

/*
void drawLetter(const uint8_t *line1, const uint8_t *line2, const uint8_t *line3, const uint8_t *line4){

  // line1
  uint8_t length1 = strlen(line1);
  for(int j=0; j<length1;j++){
      for(int i=0; i<5; i++){
       buffer[i+j*8]=FONTS[line1[j]-0x20][i];
      }
  }


  // line 2
  uint8_t length2 = strlen(line2);
  for(int j=0; j<length2;j++){
      for(int i=0; i<5; i++){
       buffer[i+128+j*8+1]=FONTS[line2[j]-0x20][i];
      }
  }

  // line 3
  uint8_t length3 = strlen(line3);
  for(int j=0; j<length3;j++){
      for(int i=0; i<5; i++){
       buffer[i+2*128+j*8+2]=FONTS[line3[j]-0x20][i];
      }
  }

 //line4
  uint8_t length4 = strlen(line4);
  for(int j=0; j<length4;j++){
      for(int i=0; i<5; i++){
       buffer[i+3*128+j*8+3]=FONTS[line4[j]-0x20][i];
      }
  }
}
*/

void drawLetter(const uint8_t *line1, const uint8_t *line2, const uint8_t *line3, const uint8_t *line4,
                const uint8_t *line5, const uint8_t *line6, const uint8_t *line7, const uint8_t *line8){

  // line1
  uint8_t length1 = strlen(line1);
  for(int j=0; j<length1;j++){
      for(int i=0; i<5; i++){
       buffer[i+j*8]=FONTS[line1[j]-0x20][i];
      }
  }


  // line 2
  uint8_t length2 = strlen(line2);
  for(int j=0; j<length2;j++){
      for(int i=0; i<5; i++){
       buffer[i+128+j*8+1]=FONTS[line2[j]-0x20][i];
      }
  }

  // line 3
  uint8_t length3 = strlen(line3);
  for(int j=0; j<length3;j++){
      for(int i=0; i<5; i++){
       buffer[i+2*128+j*8+2]=FONTS[line3[j]-0x20][i];
      }
  }

 //line 4
  uint8_t length4 = strlen(line4);
  for(int j=0; j<length4;j++){
      for(int i=0; i<5; i++){
       buffer[i+3*128+j*8+3]=FONTS[line4[j]-0x20][i];
      }
  }
  // line5
  uint8_t length5 = strlen(line5);
  for(int j=0; j<length5;j++){
      for(int i=0; i<5; i++){
       buffer[i+4*128+j*8+4]=FONTS[line5[j]-0x20][i];
      }
  }


  // line 6
  uint8_t length6 = strlen(line6);
  for(int j=0; j<length6;j++){
      for(int i=0; i<5; i++){
       buffer[i+5*128+j*8+5]=FONTS[line6[j]-0x20][i];
      }
  }

  // line 7
  uint8_t length7 = strlen(line7);
  for(int j=0; j<length7;j++){
      for(int i=0; i<5; i++){
       buffer[i+6*128+j*8+6]=FONTS[line7[j]-0x20][i];
      }
  }

 //line 8
  uint8_t length8 = strlen(line8);
  for(int j=0; j<length8;j++){
      for(int i=0; i<5; i++){
       buffer[i+7*128+j*8+7]=FONTS[line8[j]-0x20][i];
      }
  }
}


void drawChar(){

  for(int i=0; i<CACHE_SIZE_MEM; i++){
       buffer[i]=0x0F;
  }

}

