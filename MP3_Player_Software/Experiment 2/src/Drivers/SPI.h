/*------------------------------------------------------------------*-

   Port.H (v1.00)

  ------------------------------------------------------------------

   SPI Header - Exp#
   
   Programmer(s): Anderw Van Horn, Jacob Lipina, David Hinton
   
   Date: 10/1/19

   Description: Manages SPI Transfers
-------------------------------------------------------------------*/

#ifndef _SPI_H
#define _SPI_H

#include "../Main.h"

uint8_t SPI_Init(uint32_t clock_rate);
uint8_t SPI_Transfer(uint8_t send_value, uint8_t *received_value);

#define CPOL 0
#define CPHA 0

#endif