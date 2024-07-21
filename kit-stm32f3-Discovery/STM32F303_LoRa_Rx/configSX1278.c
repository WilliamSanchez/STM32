#include "configSX1278.h"
#include "misc.h"

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////                Initialization           //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/*
  1 -> reset device
  2 -> mode sleep
  3 -> read version     0x0C default version 0x12 -> 18
  4 -> set power
  5 -> set frequency    0x6C80 Default 434MHz  0x06 MSB - 0x07 MIB  0x00 LSB
  6 -> set Band width
  7 -> set spread factor
  8 -> set CRC
  9 -> define base addr RX  0x00 pointer position default, register 0X0F
  10 -> define base addr Tx  0x80 pointer position default, register 0X0E
  11 -> mode stndby
*/
////////////////////////////////////////////////////////////////////////////////////////////


/// Initialization

uint8_t SX1278_Begin(SX1278_Device *Device,
		long frequency, 
	       	uint8_t power,
  		uint8_t LoRa_Rate,
		uint8_t LoRa_BW,
		uint8_t packetLength){

  Init_Reset();	// configure the GPIO pin out to reset Lora
  reset(); 		// to do resete 
  //TxDone(); 		// Configure the GPIO pin intrupt
  //Init_SPI_LoRa(); 	// Configurate the protocol SPI

  //1->  Mode Sleep
  SX1278_Sleep(Device);  
  //2-> Read version
  if(SPI_readReg(0x42) != 18) return 1; 
  //3-> Set Mode (LORA or FSK)
  SPI_writeReg(writeReg | 0x01, MODE_LoRa); // LoRa Mode, REG_OP_MODE register pagine 108; 
  SX1278_Standby(Device); // standby mode 

  if(LoRa_Rate == 6){
	uint8_t tmp;
	// 	Highest data rate transmission
	// The header must be set to implicit mode							0x1D
	SPI_writeReg(writeReg | 0x1D, (LoRa_BW << 4) + (SX1278_CR_4_8 << 1) + 0x01);
	// Set SpreadingFactor = 6 in RegModemConfig2							0X1E
	SPI_writeReg(writeReg | 0x1E, (LoRa_Rate << 4) + (SX1278_CRC << 2) + 0x03);
  	// Set the bit field DetectionOptimize of register RegLoRaDetectOptimize to value 0b101		0x31
	tmp = SPI_readReg(0x31);
	tmp &= 0xF8;
	tmp |= 0x05;
	SPI_writeReg(writeReg | 0x31, tmp);
	// Write 0x0C in the register regDetectionThreshold						0x37
	SPI_writeReg(writeReg | 0x37, 0x0C);   
  }else{
	//Explicit Enable CRC Enable(0x02)
        //Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
	SPI_writeReg(writeReg | 0x1D, (LoRa_BW << 4) + (SX1278_CR_4_8 << 1) + 0x00);
	SPI_writeReg(writeReg | 0x1E, (LoRa_Rate << 4) + (SX1278_CRC << 2) + 0x03);
    SPI_writeReg(writeReg | 0x31, 0xc3);
    SPI_writeReg(writeReg | 0x37, 0x0a);
  }

  SPI_writeReg(writeReg | 0x26, 0x08 | 0x04); // set AGC

  // if(){SX1278_writeReg(writeReg | 0x26, SX1278_readReg(0x31) | 0x08);} // if Ts > 16ms
  SPI_writeReg(writeReg | 0x0C, SPI_readReg(0x0C) |  0x23); //RegLNA,High & LNA Enable 
  SPI_writeReg(writeReg | 0x09, power); //Setting output power parameter |Pa-Boost | (0 ---> 14) |



    setFrequency(frequency);   // Set frequency
 // SX1278_writeReg(writeReg | 0x20, 0x00); //RegPreambleMsb
 // SX1278_writeReg(writeReg | 0x21, 12); //RegPreambleLsb 8+4=12byte Preamble

  SPI_writeReg(writeReg | 0x0E, 0x00); //  set base addresses TX
  SPI_writeReg(writeReg | 0x0F, 0x00); // set base address RX
  //SPI_writeReg(writeReg | 0x39, 0x22);  // ----->>> SetSyncword
  Device->readBytes = 0;
  SX1278_Sleep(Device);  /// Mode Sleep
  delay_us(500);
  //4-> Set frequency
  //5-> Set Bandwidth
  //6-> Set SpreadFactor
  //7-> Set CRC
  //9-> Define base address Rx 0x00
  //10-> Define base address Tx 0x00

  return SPI_readReg(0x42);
}

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////               Transmition               //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/*

  begin()  --> Initializate data

  1 -> veriry if the device is stndby mode
  2 -> charger data to fifo 0x00
  3 -> clear flag of Device TxDone reg 0x12, Txdone => 0x80
  4 -> set the payload length
  5 -> set address pointer over reg fifo Tx base
  

  send()  --> send data

  6 -> set mode TX and mode LoRa  0x80 => status Tx 0x03 
  7 -> read flag Irq register TxDone 
  8 -> clear IRQ flag => 0x08
  9 -> set mode SandBy

*/
////////////////////////////////////////////////////////////////////////////////////////////
uint8_t SX1278_beginTx(SX1278_Device *Device, uint8_t *data, int length){
  
  uint8_t addr;

  SX1278_Standby(Device);  // put in standby mode
  delay_us(1000);
  SPI_writeReg(writeReg | 0x40, 0x01);  //  DIO0 --> TxDone
  SPI_writeReg(writeReg | 0x12, 0xFF);
  SPI_writeReg(writeReg | 0x11, 0xF7);
  addr = SPI_readReg(0x0E); 
  SPI_writeReg(writeReg | 0x0D, addr);
  SPI_writeData(writeReg | 0x00, data, length);
  SPI_writeReg(writeReg | 0x22, length);

  return SPI_readReg(0x39); 

}

/////////////////////////////////////////////////////////////////////////////////////////
uint8_t SX1278_sendData(SX1278_Device *Device){

  SX1278_TX(Device);
  delay_us(1000);
//  while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)){}
  while((SPI_readReg(0x12) & 0x08)  == 0){}
  SPI_writeReg(writeReg | 0x12, 0xFF);
  SX1278_Standby(Device);

  return SPI_readReg(0x39);

}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////               Receiver               //////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/*

  available()  --> Initializate data


  1 ->  assign memory (memset(---));
   pointer last address of data received and save 
   pointer register (RegFifoAddrPtr 0x??) to last address saved
   ask if it is implicit or explicit mode
   save the information received from reg 0x00 inside of struct
   save the packet size in the struct  
   clear flag IRQ LoRa module
   return number of bits read

 
  2 -> 
  3 -> 
  4 -> 
  5 -> 
  

  read_data()  --> send data

  copy struct data in a variable pointer (memcpy(---))
  as the read was read, the size of struct is zero.
  return number of length read 

  6 -> set mode TX and mode LoRa  0x80 => status Tx 0x03 
  7 -> read flag Irq register TxDone 
  8 -> clear IRQ flag => 0x08
  9 -> set mode SandBy

*/
////////////////////////////////////////////////////////////////////////////////////////////

uint8_t SX1278_available(SX1278_Device *Device, uint8_t LoRa_Rate){

  int packetLength = 0;
  int irqFlags = (SPI_readReg(0x12));

  SPI_writeReg(writeReg | 0x12, irqFlags);        // clear IRQ's


  if ((irqFlags & 0x40) && (irqFlags & 0x20) == 0){

 if((irqFlags & 0x20) == 0){
	GPIO_SetBits(GPIOE,GPIO_Pin_13);
  }
	
        memset(Device->rxBuffer, 0x00, SX1278_MAX_PACKET);

   	if (LoRa_Rate == 6) {			///    ask if it is implicit or explicit mode
      		packetLength = SPI_readReg(0x22);
    	} else {
      		packetLength = SPI_readReg(0x13);
    	}
 
       SPI_writeReg(writeReg | 0x0D, SPI_readReg(0x10)); // set FIFO address to current RX address
     
       for(int i = 0; i < packetLength; i++ ){
		Device->rxBuffer[i] = SPI_readReg(0x00); 
	}

      SPI_writeReg(writeReg | 0x01, MODE_LoRa | MODE_STDBY);
      Device->readBytes = packetLength;

     return 1;

   }else if(SPI_readReg(0x01) != (0x80 | 0x06)){
    
    SPI_writeReg(writeReg | 0x0D, 0x00);         // reset FIFO address
    SPI_writeReg(writeReg | 0x01, 0x80 | 0x06);      // put in single RX mode

  }
  
  return 0;

}

////  Send Data received to user space

uint8_t SX1278_readData(SX1278_Device *Device, uint8_t *data_read){

     if ( SX1278_available(Device, SX1278_LORA_SF_12) != 0 ){
       memcpy(data_read, Device->rxBuffer, Device->readBytes); 
       return 1;
     };
   *data_read = '\0';
   return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
//				MODES
////////////////////////////////////////////////////////////////////////////////////////////

/// Mode SLEEP

void SX1278_Sleep(SX1278_Device *Device){
  SPI_writeReg(writeReg | 0x01, MODE_SLEEP);
  Device->status = SLEEP;

}

/// Mode STANDBY

void SX1278_Standby(SX1278_Device *Device){
  SPI_writeReg(writeReg | 0x01, MODE_STDBY);
  Device->status = STANDBY;

}

/// Mode Tx
void SX1278_TX(SX1278_Device *Device){
  SPI_writeReg(writeReg | 0x01, MODE_TX);
  Device->status = TX;
}

/// Misc

void setFrequency(long frequency){			// setting frequency, page 109;	

  uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

  SPI_writeReg(writeReg | 0x06, (uint8_t)(frf >> 16));
  SPI_writeReg(writeReg | 0x07, (uint8_t)(frf >> 8));
  SPI_writeReg(writeReg | 0x08, (uint8_t)(frf >> 0));
}
