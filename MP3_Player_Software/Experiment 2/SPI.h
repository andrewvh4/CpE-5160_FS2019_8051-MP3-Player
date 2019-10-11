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

#include "Main.h"
#include "Port.h"

uint8_t SPI_Init(uint32_t clock_freq);
uint8_t SPI_Transfer(uint8_t send_value, uint8_t *received_value);
uint8_t SPI_setCSState(uint8_t state);

#define CPOL 0
#define CPHA 0
#define CS P1_4

#define SPI_NO_ERROR 0 
#define SPI_ERROR 1
#define SPI_ERROR_TIMEOUT 2

#define SPI_ERROR_CLOCKRATE 3


#endif