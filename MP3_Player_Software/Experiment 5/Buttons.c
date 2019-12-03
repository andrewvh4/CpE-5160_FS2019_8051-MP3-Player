#include "Buttons.h"


void Button_initParams()
{
	// Setup pinmask and button state for all 4 buttons
	// Configure switch values to represent the initial state
	for (int i = 0; i < 4; i++)
	{
		butttons[i].buttonID = i;
		buttons[i].inputMask = 1 << 0;
		buttons[i].debounceTime = DEBOUNCE_TIME_ms;
		buttons[i].state = NOT_PRESSED;
	}
}

bool Button_allOff()
{	
	bool allOff = true;

	for (int i = 0; i < 4; i++)
	{
		if(buttons[i].state != NOT_PRESSED)
		{
			allOff = false;
		}
	}
	return allOff;
}

void Button_Read(button& aButton)
{
	switch(aButtonState)
	{	
		case NOT_PRESSED:
		{
			if((Input_Port & aButton.state) == 0)
			{
				aButton.debounceTime = 0;
				aButton.state = DEBOUNCE_P;
			}
		}
		case PRESSED:
		{
			// Put task for a pressed switch here
			// Behavior can be determined based on buttonID
		}
		case HELD:
		{
			if((Input_Port & aButton.inputMask) == 1)
			{
				aButton.state = DEBOUNCE_R;
			}
		}
		case DEBOUNCE_P:
		{
			aButton.debounceTime += interval; // interval not defined
			if (aButton.debounceTime >= DEBOUNCE_TIME_ms)
			{
				if ((Input_Port & aButton.state) == 0)
				{
					aButton.state = PRESSED;
				}
				else
				{
					aButton.state = NOT_PRESSED;
				}
			}
		}
		case DEBOUNCE_R:
		{
			aButton.debounceTime += interval;
			if (aButton.debounceTime >= DEBOUNCE_TIME_ms)
			{
				if ((Input_Port & aButton.state) == 1)
				{
					aButton.state = NOT_PRESSED;

					// Put task for a pressed switch here
				}
				else
				{
					aButton.state = HELD;
				}
			}			
		}
		default:
		{
			break;
		}
	}
}