#ifndef __LCDOLED_H__
#define __LCDOLED_H__

#include "configI2C.h"

  #define LCDHEIGHT		    32
  #define SSD1306_LCDWIDTH          128

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



void LCD_Init();

uint8_t display();
void drawLetter(const uint8_t *line1, const uint8_t *line2, const uint8_t *line3, const uint8_t *line4);
void drawChar();
void drawChar1();
void sendData();
void clearDisplay();

#endif


