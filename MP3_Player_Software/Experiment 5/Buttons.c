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
		buttons[i].debounceTime = 0;
		buttons[i].state = NOT_PRESSED;
	}
}

uint8_t Button_allOff()
{	
	uint8_t allOff = 1; //Assume all buttons are pressed
	int i;

	//If one button is pressed, clear allOff variable
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

	button_pressed = P2 & buttons[ID].inputMask; //Read state of button from ID
	buttons[ID].debounceTime += sEOS_LOOP_INTERVAL_ms; //Increment debounce time on the button

	switch(buttons[ID].state)
	{	
		case NOT_PRESSED:
			if(button_pressed == 0) //If the button is pressed
			{
				buttons[ID].state = DEBOUNCE_P; //Go to pressed debounce state
				buttons[ID].debounceTime = 0; //reset the debounce time
			}
			break;
		case PRESSED:
			if(button_pressed != 0) //If the button is not pressed
			{
				buttons[ID].state = DEBOUNCE_R; //Go to released debounce state
				buttons[ID].debounceTime = 0; //reset the dounce time
			}
			break;
		case DEBOUNCE_P:
			if(button_pressed != 0) //If button becomes not pressed
			{
				buttons[ID].state = NOT_PRESSED; //Go back to not pressed state
			}
			else if(buttons[ID].debounceTime >= DEBOUNCE_TIME_ms) //If the button has been pressed for longer than the debounce time
			{
				buttons[ID].state = PRESSED; //Go to the pressed state
			}
			break;
		case DEBOUNCE_R:
			if(button_pressed == 0) //If the button becomes pressed
			{
				buttons[ID].state = PRESSED; //Go to the pressed state
			}
			else if(buttons[ID].debounceTime >= DEBOUNCE_TIME_ms) //If the button has been released for linger than the debounce time
			{
				buttons[ID].state = NOT_PRESSED; //Go to the not pressed state
			}
			break;
		default:
			break;
	}
}
void Button_ReadAll()
{
	//Read all buttons
	Button_Read(0);
	Button_Read(1);
	Button_Read(2);
	Button_Read(3);
}
