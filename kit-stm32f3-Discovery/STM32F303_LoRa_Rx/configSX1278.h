#ifndef __CONFIGSX1278_H__
#define __CONFIGSX1278_H__

#include <stm32f30x.h>
#include "configSPI.h"

#define SX1278_MAX_PACKET		255
#define SX1278_915MHZ			915E6

// MODES

#define MODE_SLEEP			0x00
#define MODE_STDBY			0x01
#define MODE_TX				0x03
#define MODE_RX_CONTINUOUS		0x05
#define MODE_RX_SINGLE			0x06
#define MODE_LoRa			0x80

// POWER

#define SX1278_POWER_20DBM		0xFF
#define SX1278_POWER_17DBM		0xFC
#define SX1278_POWER_14DBM		0xF9
#define SX1278_POWER_11DBM		0xF6
		

// SPREADING FACTOR 		

#define SX1278_LORA_SF_6		6
#define SX1278_LORA_SF_7		7
#define SX1278_LORA_SF_8		8
#define SX1278_LORA_SF_9		9
#define SX1278_LORA_SF_10		10
#define SX1278_LORA_SF_11		11
#define SX1278_LORA_SF_12		12

// BAND WIDTH

#define	SX1278_LORA_BW_7_8KHZ		0
#define	SX1278_LORA_BW_10_4KHZ		1
#define	SX1278_LORA_BW_15_6KHZ		2
#define	SX1278_LORA_BW_20_8KHZ		3
#define	SX1278_LORA_BW_31_2KHZ		4
#define	SX1278_LORA_BW_41_7KHZ		5
#define	SX1278_LORA_BW_62_5KHZ		6
#define	SX1278_LORA_BW_125KHZ		7
#define	SX1278_LORA_BW_250KHZ		8
#define	SX1278_LORA_BW_500KHZ		9

// Error Coding rate (CR)

#define SX1278_CR_4_5			0x01
#define SX1278_CR_4_6			0x02
#define SX1278_CR_4_7			0x03
#define SX1278_CR_4_8			0x04

/*  0x|Tx|Crc|  =>  0x00 --> A single packet / Crc Disable
                    0x01 --> A single packet / Crc Enable
                    0x10 --> Tx contiuos mode / Crc Disable
                    0x11 --> Tx contiuos mode / Crc Enable

*/
#define SX1278_CRC   			0x00 

#define writeReg			0x80
#define readReg				0x7F

typedef enum {
	SLEEP, 
	STANDBY, 
	TX, 
	RX
} SX1278_Status;

typedef struct {

	uint8_t frequency;
	uint8_t power;
	uint8_t LoRa_Rate;
	uint8_t LoRa_BW;
	uint8_t packetLength;

	SX1278_Status status;

	uint8_t rxBuffer[SX1278_MAX_PACKET];
	uint8_t readBytes;

}SX1278_Device;

SX1278_Device Sx1278_LoRa;

/// Initialization

uint8_t SX1278_Begin(SX1278_Device *Device,
		long frequency, 
	       	uint8_t power,
  		uint8_t LoRa_Rate,
		uint8_t LoRa_BW,
		uint8_t packetLength);

uint8_t SX1278_confg(SX1278_Device *Device,
		long frequency, 
	       	uint8_t power,
  		uint8_t LoRa_Rate,
		uint8_t LoRa_BW,
		uint8_t packetLength);
// Transmit Data 

uint8_t SX1278_beginTx(SX1278_Device *Device, uint8_t *data, int length);
uint8_t SX1278_sendData(SX1278_Device *Device);

// Receiver Data

uint8_t SX1278_available(SX1278_Device *Device, uint8_t LoRa_Rate);
uint8_t SX1278_readData(SX1278_Device *Device,  uint8_t *data_read);

//		MODES

void SX1278_Sleep(SX1278_Device *Device);
void SX1278_Standby(SX1278_Device *Device);
void SX1278_TX(SX1278_Device *Device);
void SX1278_RX(SX1278_Device *Device);

/// Misc

void setFrequency(long frequency);

#endif
