#include "Buttons.h"
#include "Port.h"
#include "OS.h"

buttonParams buttons[4];

void Button_initParams()
{
	// Setup pinmask and button state for all 4 buttons
	// Configure switch values to represent the initial state
	int i;
	for (i = 0; i < 4; i++)
	{
		buttons[i].ID = i;
		buttons[i].inputMask = 0x01 << i;
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
	uint8_t i;
	uint8_t button_pressed;

	button_pressed = P2 & buttons[ID].inputMask;
	buttons[ID].debounceTime += sEOS_LOOP_INTERVAL_ms;

	switch(buttons[ID].state)
	{	
		case NOT_PRESSED:
			if(button_pressed == 0)
			{
				buttons[ID].state = DEBOUNCE_P;
				buttons[ID].debounceTime = 0;
			}
			break;
		case PRESSED:
			if(button_pressed != 0)
			{
				buttons[ID].state = DEBOUNCE_R;
				buttons[ID].debounceTime = 0;
			}
			break;
		case DEBOUNCE_P:
			if(button_pressed != 0)
			{
				buttons[ID].state = NOT_PRESSED;
			}
			else if(buttons[ID].debounceTime >= DEBOUNCE_TIME_ms)
			{
				buttons[ID].state = PRESSED;
			}
			break;
		case DEBOUNCE_R:
			if(button_pressed == 0)
			{
				buttons[ID].state = PRESSED;
			}
			else if(buttons[ID].debounceTime >= DEBOUNCE_TIME_ms)
			{
				buttons[ID].state = NOT_PRESSED;
			}
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
