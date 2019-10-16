/*------------------------------------------------------------------*-

   SDCard.h (v1.00)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-10-01

   Description: Defines functions SD Card operation
-*------------------------------------------------------------------*/
#include "../Main.h"
#ifndef _SDCard_H
#define _SDCard_H

// ------ Public function prototypes -------------------------------
uint8_t SD_Init();
uint8_t SD_readBlock(uint32_t block_number, uint16_t num_bytes, uint8_t * array_out);
uint8_t SD_sendCommand(uint8_t CMD_value, uint32_t argument);
uint8_t SD_receiveResponse(uint8_t num_bytes, uint8_t * rec_array);

//------- Constants ------------------------------------------------
#define NO_ERRORS 0
#define SD_ERROR_ILLEGAL_COMMAND 1
#define SD_ERROR_TIMEOUT 3
#define SD_ERROR_COMM 4
#define SD_ERROR_RESPONSE 5
#define SD_ERROR_INVALID_DATA_START_TOKEN 6 
#define SD_ERROR_BAD_RESPONSE 7
#define SD_ERROR_VOLTAGE 8
#define SD_ERROR_DATA 9

#define SCK_INIT_BYTES 10


#define CMD0 0
#define CMD8 8
#define CMD17 17
#define CMD55 55
#define CMD58 58
#define ACMD41 41

#define CARD_TYPE_UNKNOWN (0xFF)
#define CARD_TYPE_STANDARD_CAPACITY (9)
#define CARD_TYPE_VERSION_2 (0x02)
#define CARD_TYPE_HIGH_CAPACITY (0) 

#endif