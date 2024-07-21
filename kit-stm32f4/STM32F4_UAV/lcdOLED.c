#include "stm32f4xx.h"
#include "lcdOLED.h"
#include <string.h>
#include "fonts.h"

#define mask 0x00000000000000FF

static uint8_t buffer[CACHE_SIZE_MEM];
extern uint8_t contDelay;
extern uint8_t conelevator;
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

void drawLetter(uint8_t *elevator){

   //////////// 	Marco
   for(int i=0; i<8; i++)
   {
     	buffer[i*128+i]=0xFF;buffer[i*128+i+1]=0xFF;buffer[i*128+i+2]=0xFF;
     	buffer[(i+1)*128+(i-1)-2]=0xFF;buffer[(i+1)*128+(i-1)-1]=0xFF;buffer[(i+1)*128+(i-1)]=0xFF;
   }
   
   for(int i=3; i<125; i++)
   {
     	buffer[i]=0x0F;	buffer[7*128+7+i]=0xF0; 
   }
   /////////////////////////////////////
   
   conelevator = (uint8_t)(48*readData_ADC1()/3300);
   
   uint64_t indc = 0x00ff000000000000>>conelevator;
   uint64_t indc2 = ~(0x0000ffffffffffff>>conelevator);
   uint64_t indc3 = 0xfff1e18181e1f1ff;
   uint64_t indc4 = 0x00007e7e7e7e0000;
   uint64_t indc5 = 0xffe7c38181818181;
   
   uint64_t col1 = 0x0000ffffffffffff;
   uint64_t col2 = 0x0000800000000001;
    
   uint8_t cont = 40+contDelay;
   uint8_t cont2 = 40+contDelay;
    
   for(int i=0; i<8; i++)
   {
   
     buffer[(3+1)*128+(3+1)+16+i]= (uint8_t)(indc5>>i*8)&mask;
     
     if(i<6){
     
     //LEFT
     	buffer[(i+1)*128+(i+1)+8]= (uint8_t)((col1|indc)>>i*8)&mask;
     	buffer[(i+1)*128+(i+1)+9]= (uint8_t)((col2|indc)>>i*8)&mask;
     	buffer[(i+1)*128+(i+1)+10]= (uint8_t)((col2|indc)>>i*8)&mask;
     	buffer[(i+1)*128+(i+1)+11]= (uint8_t)((col2|indc)>>i*8)&mask;
     	buffer[(i+1)*128+(i+1)+12]= (uint8_t)((col2|indc)>>i*8)&mask;
     	buffer[(i+1)*128+(i+1)+13]= (uint8_t)((col2|indc)>>i*8)&mask;
     	buffer[(i+1)*128+(i+1)+14]= (uint8_t)((col2|indc)>>i*8)&mask;
     	buffer[(i+1)*128+(i+1)+15]= (uint8_t)((col1|indc)>>i*8)&mask;
     
     	buffer[(i+1)*128+(i+1)+16]= (uint8_t)(((indc>>i*8)&mask)&(~indc5)>>8*0);
     	buffer[(i+1)*128+(i+1)+17]= (uint8_t)(((indc>>i*8)&mask)&(~indc5)>>8*1);
     	buffer[(i+1)*128+(i+1)+18]= (uint8_t)(((indc>>i*8)&mask)&(~indc5)>>8*2);
     	buffer[(i+1)*128+(i+1)+19]= (uint8_t)(((indc>>i*8)&mask)&(~indc5)>>8*3);
     	buffer[(i+1)*128+(i+1)+20]= (uint8_t)(((indc>>i*8)&mask)&(~indc5)>>8*4);
     	buffer[(i+1)*128+(i+1)+21]= (uint8_t)(((indc>>i*8)&mask)&(~indc5)>>8*5);
     	buffer[(i+1)*128+(i+1)+22]= (uint8_t)(((indc>>i*8)&mask)&(~indc5)>>8*6);
     	buffer[(i+1)*128+(i+1)+23]= (uint8_t)(((indc>>i*8)&mask)&(~indc5)>>8*7);
     
     //RIGTH
     
     	buffer[(i+2)*128+(i)-15]= (uint8_t)((col1|indc2)>>i*8)&mask;
     	buffer[(i+2)*128+(i)-14]= (uint8_t)((col2|indc2)>>i*8)&mask;
     	buffer[(i+2)*128+(i)-13]= (uint8_t)((col2|indc2)>>i*8)&mask;
     	buffer[(i+2)*128+(i)-12]= (uint8_t)((col2|indc2)>>i*8)&mask;
     	buffer[(i+2)*128+(i)-11]= (uint8_t)((col2|indc2)>>i*8)&mask;
     	buffer[(i+2)*128+(i)-10]= (uint8_t)((col2|indc2)>>i*8)&mask;
     	buffer[(i+2)*128+(i)-9]= (uint8_t)((col2|indc2)>>i*8)&mask;
     	buffer[(i+2)*128+(i)-8]= (uint8_t)((col1|indc2)>>i*8)&mask;
     }
   }
   
    buffer[6*128+6+25]=0xFF; buffer[(6+1)*128+(6-1)-25]=0xFF;
   for(int i=26; i<128-26; i++)
   {
     if(i>(128-26+26)/2-4 && i<=(128-26+26)/2+4)
     {
        buffer[1*128+1+i] = (uint8_t)((~indc4>>(8*(i-((128-26+26)/2-4)-1)))&mask);
        buffer[6*128+6+i] = (uint8_t)((indc3>>(8*(i-((128-26+26)/2-4)-1)))&mask);
     }else{
        buffer[1*128+1+i] = 0x18;
        buffer[6*128+6+i] = 0x81;
     }
     
     if (i>cont2 && i<=cont2+8)
     	buffer[1*128+1+i] =  buffer[1*128+1+i]|(uint8_t)((indc4>>(8*(i-cont2-1)))&mask);     
     
     if (i>cont && i<=cont+8)
     	buffer[6*128+6+i] = buffer[6*128+6+i]|(uint8_t)((~indc3>>(8*(i-cont-1)))&mask);
   }
}


/*
  // line1
  buffer[0]=0xFF; 
  buffer[127]=0xFF;
     
  //line 2
   buffer[128+1]=0xFF; //buffer[128+1]=0xFF; buffer[128+2]=0xFF;
   buffer[2*128]=0xFF; //buffer[2*128-2]=0xFF; buffer[2*128-1]=0xFF;
   
   //line 3
   buffer[2*128+2]=0xFF; //buffer[128+1]=0xFF; buffer[128+2]=0xFF;
   buffer[3*128+1]=0xFF; //buffer[2*128-2]=0xFF; buffer[2*128-1]=0xFF;
   
   //line 4
   buffer[3*128+3]=0xFF; //buffer[128+1]=0xFF; buffer[128+2]=0xFF;
   buffer[4*128+2]=0xFF; //buffer[2*128-2]=0xFF; buffer[2*128-1]=0xFF;
   
   //line 5
   buffer[4*128+4]=0xFF; //buffer[128+1]=0xFF; buffer[128+2]=0xFF;
   buffer[5*128+3]=0xFF; //buffer[2*128-2]=0xFF; buffer[2*128-1]=0xFF;
   
   //line 6
   buffer[5*128+5]=0xFF; //buffer[128+1]=0xFF; buffer[128+2]=0xFF;
   buffer[6*128+4]=0xFF; //buffer[2*128-2]=0xFF; buffer[2*128-1]=0xFF;
   
   //line 7
   buffer[6*128+6]=0xFF; //buffer[128+1]=0xFF; buffer[128+2]=0xFF;
   buffer[7*128+5]=0xFF; //buffer[2*128-2]=0xFF; buffer[2*128-1]=0xFF;
   
   //line 8
   buffer[7*128+7]=0xFF; //buffer[128+1]=0xFF; buffer[128+2]=0xFF;
   buffer[8*128+6]=0xFF; //buffer[2*128-2]=0xFF; buffer[2*128-1]=0xFF;
*/

