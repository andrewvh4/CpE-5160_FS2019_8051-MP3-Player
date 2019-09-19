/*------------------------------------------------------------------*-

   Port.H (v1.00)

  ------------------------------------------------------------------

   Port Header - Exp#1 UART & LCD Module
   
   Programmer(s): Roger Younger 
   
   Date: 9/8/16

   Description: Defines the port pins for the basic 8051 circuit
-*------------------------------------------------------------------*/

#ifndef _PORT_H
#define _PORT_H

uint8_t writeBus(uint8_t bus, uint8_t bits, uint8_t bitMask);


#define SW1 P2_0
#define SW2 P2_1
#define SW3 P2_2
#define SW4 P2_3
#define REDLED P2_4
#define GREENLED P2_5
#define YELLOWLED P2_6
#define AMBERLED P2_7

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
uint8_t P0_0 = 0x00;
uint8_t P0_1 = 0x01;
uint8_t P0_2 = 0x02;
uint8_t P0_3 = 0x03;
uint8_t P0_4 = 0x04;
uint8_t P0_5 = 0x05;
uint8_t P0_6 = 0x06;
uint8_t P0_7 = 0x07;

// Port 1
uint8_t P1_0 = 0x10;
uint8_t P1_1 = 0x11;
uint8_t P1_2 = 0x12;
uint8_t P1_3 = 0x13;
uint8_t P1_4 = 0x14;
uint8_t P1_5 = 0x15;
uint8_t P1_6 = 0x16;
uint8_t P1_7 = 0x17;

// Port 2
uint8_t P2_0 = 0x20;
uint8_t P2_1 = 0x21;
uint8_t P2_2 = 0x22;
uint8_t P2_3 = 0x23;
uint8_t P2_4 = 0x24;
uint8_t P2_5 = 0x25;
uint8_t P2_6 = 0x26;
uint8_t P2_7 = 0x27;

// Port 3
uint8_t P3_0 = 0x30;
uint8_t P3_1 = 0x31;
uint8_t P3_2 = 0x32;
uint8_t P3_3 = 0x33;
uint8_t P3_4 = 0x34;
uint8_t P3_5 = 0x35;
uint8_t P3_6 = 0x36;
uint8_t P3_7 = 0x37;

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/



