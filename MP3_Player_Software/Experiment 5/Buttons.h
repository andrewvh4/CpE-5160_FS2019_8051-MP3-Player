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

struct buttonParams
{
	uint8_t inputMask;
	uint8_t state;
	uint16_t debounceTime
}

buttonParams buttons[4];

void Button_initParams();
void Button_updateStates();
//Returns 1 if all buttons are off. 0 if at least one button is on
uint8_t Button_allOff();


#define BUTTON_STATE_OFF 1
#define BUTTON_STATE_ON 2
#define BUTTON_STATE_RISING_DEBOUNCE 3
#define BUTTON_STATE_FALLING_DEBOUNCE 4

#define DEBOUNCE_TIME_ms 100

#define PRESSED 1
#define NOT_PRESSED 2


#endif