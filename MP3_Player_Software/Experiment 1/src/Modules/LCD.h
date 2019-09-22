/*------------------------------------------------------------------*-

   LCD.h (v1.00)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
	 
	 Hardware: LCD_LCM-S01602DTR/A
   
   Date: 2019-09-15

   Description: Defines functions for LCD operation
-*------------------------------------------------------------------*/
// ------ Public function prototypes -------------------------------
#ifndef _LCD_H
#define _LCD_H

#include "../Main.h"

uint8_t LCD_Write_String(uint8_t* string, uint8_t line);

uint8_t LCD_Init();

uint8_t LCD_Write_Data(uint8_t dat);
uint8_t LCD_Write_Command(uint8_t command);
uint8_t LCD_Write(uint8_t value, uint8_t rs_value);

//------- Constants ------------------------------------------------
#define COMMAND 0
#define DATA 1
#define LOW_POWER_STATE 0xff
#define LINE_1 0x00
#define LINE_2 0x40

#define COMMMAND 0x00 //Need to verify this, created compile error

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/



