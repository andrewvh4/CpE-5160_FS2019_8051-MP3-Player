/*--------------------------------------------------------------------

   Buttons.h (v1.01)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-12-01

   Description: Defines functions Button states and debounce
--------------------------------------------------------------------*/

#ifndef _BUTTONS
#define _BUTTONS

#include "Main.h"

typedef enum
{
   NOT_PRESSED,
   PRESSED,
   DEBOUNCE_P,
   DEBOUNCE_R
}switch_state_t;

typedef struct
{
  	uint8_t ID;
	uint8_t inputMask;
  	uint16_t debounceTime;
	switch_state_t state;	
} buttonParams;

extern buttonParams buttons[4];

void Button_initParams();

// Returns true if all buttons are off
// Returns false if at least one button is on
uint8_t Button_allOff();

void Button_Read(uint8_t ID);
void Button_ReadAll();

#define DEBOUNCE_TIME_ms 100

#endif