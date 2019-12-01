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

buttonParams buttons[4];

void Button_initParams();
void Button_updateStates();


struct buttonParams
{
	uint8_t inputMask;
	uint8_t buttonState;
}

#define BUTTON_STATE_OFF 1
#define BUTTON_STATE_ON 2
#define BUTTON_STATE_RISING_DEBOUNCE 3
#define BUTTON_STATE_FALLING_DEBOUNCE 4


#endif