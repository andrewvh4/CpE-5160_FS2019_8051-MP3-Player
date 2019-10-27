/*------------------------------------------------------------------*-

   I2C.H (v1.00)

  ------------------------------------------------------------------

   I2C Header - Exp3
   
   Programmer(s): Anderw Van Horn, Jacob Lipina, David Hinton
   
   Date: 10/12219

   Description: Manages SPI Transfers
-------------------------------------------------------------------*/

#ifndef _I2C_H
#define _I2C_H

#include "../Main.h"
#include "Port.h"
#include "AT89C51RC2.h"

#define I2C_NO_ERROR 0
#define I2C_ACK_ERROR 1
#define I2C_BUS_BUSY_ERROR 2

uint8_t I2C_Read(uint8_t address, uint32_t int_address, uint8_t int_address_size, uint8_t num_bytes,uint8_t * ret_array);
uint8_t I2C_Write(uint8_t address, uint32_t int_address, uint8_t int_address_size, uint8_t num_bytes,uint8_t * write_array);

void I2C_Clock_Start();
void I2C_Clock_Delay(uint8_t control);

sbit SDA = P1^5;
sbit SCL = P1^6;

#endif