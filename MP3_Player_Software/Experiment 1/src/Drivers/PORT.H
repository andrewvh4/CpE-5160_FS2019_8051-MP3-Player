/*------------------------------------------------------------------*-

   Port.H (v1.00)

  ------------------------------------------------------------------

   Port Header - Exp#1 UART & LCD Module
   
   Programmer(s): Roger Younger 
   
   Date: 9/8/16

   Description: Defines the port pins for the basic 8051 circuit
-------------------------------------------------------------------*/

#ifndef _PORT_H
#define _PORT_H

#include "../Main.h"


uint8_t Port_writeBus(uint8_t bus, uint8_t bits, uint8_t bitMask);
uint8_t Port_writePin(uint8_t pin, uint8_t value);

uint8_t Port_readBus(uint8_t bus);
uint8_t Port_readPin(uint8_t pin);

#define HIGH 1
#define LOW 0

#define LCD_D0 P0_0
#define LCD_D1 P0_1
#define LCD_D2 P0_2
#define LCD_D3 P0_3
#define LCD_D4 P0_4
#define LCD_D5 P0_5
#define LCD_D6 P0_6
#define LCD_D7 P0_7

#define LCD_RS P3_5
#define LCD_EN P3_7

#define per_TxD P3_0
#define per_RxD P3_1

// Port 0
#define P0_0 0x00
#define P0_1 0x01
#define P0_2 0x02
#define P0_3 0x03
#define P0_4 0x04
#define P0_5 0x05
#define P0_6 0x06
#define P0_7 0x07

// Port 1
#define P1_0 0x10
#define P1_1 0x11
#define P1_2 0x12
#define P1_3 0x13
#define P1_4 0x14
#define P1_5 0x15
#define P1_6 0x16
#define P1_7 0x17

// Port 2
#define P2_0 0x20
#define P2_1 0x21
#define P2_2 0x22
#define P2_3 0x23
#define P2_4 0x24
#define P2_5 0x25
#define P2_6 0x26
#define P2_7 0x27

// Port 3
#define P3_0 0x30
#define P3_1 0x31
#define P3_2 0x32
#define P3_3 0x33
#define P3_4 0x34
#define P3_5 0x35
#define P3_6 0x36
#define P3_7 0x37

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/



