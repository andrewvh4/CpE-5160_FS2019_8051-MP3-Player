#include "Buttons.h"
#include "Port.h"

buttonParams buttons[4];

void Button_initParams()
{
	// Setup pinmask and button state for all 4 buttons
	// Configure switch values to represent the initial state
	int i;
	for (i = 0; i < 4; i++)
	{
		buttons[i].ID = i;
		buttons[i].inputMask = 1 << 0;
		buttons[i].debounceTime = DEBOUNCE_TIME_ms;
		buttons[i].state = NOT_PRESSED;
	}
}

uint8_t Button_allOff()
{	
	uint8_t allOff = 1;
	int i;

	for (i = 0; i < 4; i++)
	{
		if(buttons[i].state != NOT_PRESSED)
		{
			allOff = 0;
		}
	}
	return allOff;
}

void Button_Read(uint8_t ID)
{
	buttonParams aButton;
	uint8_t i;
	uint8_t button_pressed;

	aButton = buttons[ID];

	button_pressed = P2 & aButton.inputMask;

	switch(aButton.state)
	{	
		case NOT_PRESSED:
			break;
		case PRESSED:
			break;
		case DEBOUNCE_P:
			break;
		case DEBOUNCE_R:
			break;
		default:
			break;
	}
}
void Button_ReadAll()
{
	Button_Read(0);
	Button_Read(1);
	Button_Read(2);
	Button_Read(3);
}
