/*------------------------------------------------------------------*-

   SDCard.h (v1.00)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-10-01

   Description: Defines functions SD Card operation
-*------------------------------------------------------------------*/
#include "Main.h"
#ifndef _SDCard_H
#define _SDCard_H

// ------ Public function prototypes -------------------------------
uint8_t SD_Init();
uint8_t SD_readBlock(uint16_t block_number, uint16_t num_bytes, uint8_t * array_out);
uint8_t SD_sendCommand(uint8_t CMD_value, uint32_t argument);
uint8_t SD_receiveResponse(uint8_t num_bytes, uint8_t * rec_array);

//------- Constants ------------------------------------------------
#define NO_ERRORS 0
#define ILLEGAL_COMMAND 1
#define SD_TIMEOUT_ERROR 3
#define COMM_ERROR 4
#define RESPONSE_ERROR 5
#define SD_ERROR_INVALID_DATA_START_TOKEN 6 
#define SD_ERROR_BAD_RESPONSE 7


#define CMD0 0
#define CMD8 8
#define CMD17 17

#endif