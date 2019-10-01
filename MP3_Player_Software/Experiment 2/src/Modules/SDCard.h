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


uint8_t SD_Init();
uint8_t SD_readBlock(uint16_t number_of_bytes, uint8_t * array);


#endif