#ifndef _LCD_H
#define _LCD_H

/*------------------------------------------------------------------*-

   LCD.h (v1.00)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
	 
	 Hardware: LCD_LCM-S01602DTR/A
   
   Date: 2019-09-15

   Description: Defines functions for LCD operation
-*------------------------------------------------------------------*/
#ifndef _LCD_H
#define _LCD_H
// ------ Public function prototypes -------------------------------
uint8_t LCD_Write_String(uint8_t* string, uint8_t line, uint8_t starting_char);

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

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/



