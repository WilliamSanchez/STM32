#include "ST7796.h"



const uint8_t YMLBs = (Vertical_line >> 8) & 0xFF;
const uint8_t YLSBs = Vertical_line & 0xFF;
const uint8_t XMLBs = (Horizontal_line >> 8) & 0xFF;
const uint8_t XLSBs = Horizontal_line & 0xFF;

uint8_t reg[] = {0x80, 0x02, 0x3B};
uint8_t col[] = {0x00, 0x00, XMLBs, XLSBs};
uint8_t row[] = {0x00, 0x00, YMLBs, YLSBs};

void config_command()
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  // Enable the GPIO_LED Clock //
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // Configure the GPIO_LED pin //
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  //GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
}

int8_t st7796_init()
{
    config_command();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // ---> nss 
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // -->> Write comnad
    SPI_TxData(0x01);
    HAL_Delay(150);
    /*
      F0h Comand Set Control pag 239
      
      C3h: Enable command part I
      96h: Enable command part II

      3Ch: Disable command part I
      69h Disable command part II
    */
    SPI_writeReg(0xF0, 0xC3);
    SPI_writeReg(0xF0, 0x96);
    /*
      C5h VCOM Control pag 227.
    */
    SPI_writeReg(0xC5, 0x1C);
    /*
      36h MADCTL Memory Data Access Control pag 183

      MY|MX|MV: These 3bits controls MCU to memory write/read direction.
      ML:       Vertical refresh order "0" Top to Bottom, "1" Bottom to Top
      RGB:      Color selector switch "0" RGB, "1" BGR.
      MH:       Horizontal refresh order '0' Left to Right, '1' Right to Left
    */
    SPI_writeReg(0x36, 0x48);
    /*
      3Ah Interface pixel format pag 190
      
      D7-D4: RGB Interface color format '0101' 16bit/pixel, '0110' 18bit/pixel
      D3-D0: Control interface color format '0101' 16bit/pixel, '0110' 18bit/pixel, '0111' 24 bit/pixel.
    */
    SPI_writeReg(0x3A, 0x55);  
    /*
      B0h Interface moed Control pag 212

      SPI_EN: 3/4 wire wire serial interface selection, '0' DIN and DOUT pins are used for 3/4 wire serial interface, 
                                                        '1' DIN/SDA pin is used for 3/4 wire serial interface and DOUT pin is not used

      DEP: polarity '0' High enable for RGB Interface, '1' Low enbale for RGB interface
      PKP: PCLK polarity '0' rising time, '1' falling time.
      HSCP: HSYNC Polarity '0' low level sync clock, '1' High level sync clock.
      VSCP: VSYNC polarity '0' low level sync clock, '1' Higj level sync clock.
    */
    SPI_writeReg(0xB0, 0x80);
    /*
      B4h Dislay inversion control pag 216

      '00' Column inversion, '01' 1-dot inversion, '10' 2-dot inversion
    */
    SPI_writeReg(0xB4, 0x00);
    /*
      B6h Display fnction control pag 218
    */
    SPI_writeData(0xB6, reg, 3);
    /*
      B7h Entry mode set pag 222
    */
    SPI_writeReg(0xB7, 0xC6);
    SPI_writeReg(0xF0, 0x69);
    SPI_writeReg(0xF0, 0x3C);
    /*
      2Ah Column address set pag 170.

      Xs[15:0]: Start
      Xe[15:0]: End
    */
    SPI_writeData(COL_ADDR_SET,col, 4);
      /*
      2Bh Row address set pag 172.

      ys[15:0]: Start
      ye[15:0]: End
    */
    SPI_writeData(ROW_ADDR_SET, row, 4);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // -->> Write comnad
    /*
      11h Sleep OUT ag 160.

      Turn off sleep out.
    */
    SPI_TxData(0x11);
    HAL_Delay(150);
    /*
      29h Display ON pag 169
      This command is used to recover from display off mode 28h
    */
    SPI_TxData(0x29); 
    HAL_Delay(150);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // ---> nss 

    return 0;
}