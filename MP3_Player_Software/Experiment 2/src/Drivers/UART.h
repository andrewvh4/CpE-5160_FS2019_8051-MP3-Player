/*------------------------------------------------------------------*-

   UART.h (v1.00)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-09-15

   Description: Defines functions for serial operation
-*------------------------------------------------------------------*/

#ifndef _UART_H
#define _UART_H

#define SMOD1 (0)
#define SPD   (1)

// ------ Public function prototypes -------------------------------

uint8_t UART_Init_9600();
uint8_t UART_Init(uint8_t baudRate);

uint8_t UART_Transmit(uint8_t character);

uint8_t UART_Receive();

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/



