#include "OSFunctions.h"

uint8_t LED_State = 0;//Stores what state the display is in for flashLR and flashRL
uint8_t LED_Display = 0x00;//Stores the current state of the LED display
uint8_t current_state = STATE_OFF;//Current state of the system
uint16_t LED_Delay_ms = LED_MAX_VALUE;//current LED delay
uint8_t LED_speed = 15;//Current LED speed counter (set to 15 for max)
uint16_t timeCounter = 0; //stores elapsed system time
uint8_t delay_state = DELAY_STATE_WAIT_P; //Stores state for delay adjustment state

void flashLR();
void flashRL();
void delayAdj();

void App_UpdateState()
{
	timeCounter += sEOS_LOOP_INTERVAL_ms; //Increment elapsed system time
	switch(current_state)
	{
		case STATE_OFF_P:
			if(Button_allOff()) current_state = STATE_OFF; //If all buttons are off, go to state_off
			LED_State = 0; //Reset LED state
			LED_Display = 0x00; //Clear LED display
			delay_state = DELAY_STATE_WAIT_P; //Reset delay state
			break;
		case STATE_OFF://Go to new state based on buttons pressed
			if(buttons[0].state == PRESSED) current_state = STATE_FLASH_LR_P;
			else if(buttons[1].state == PRESSED) current_state = STATE_DELAY_ADJ_P;
			else if(buttons[2].state == PRESSED) current_state = STATE_DELAY_ADJ_P;
			else if(buttons[3].state == PRESSED) current_state = STATE_FLASH_RL_P;
			break;
		case STATE_FLASH_LR_P: //If all butons are off, go to next state
			if(Button_allOff()) current_state = STATE_FLASH_LR;
			break;
		case STATE_FLASH_LR: //If button 0 is pressed again, go to off state
			if(buttons[0].state == PRESSED) current_state = STATE_OFF_P;
			break;
		case STATE_FLASH_RL_P://If all butons are off, go to next state
			if(Button_allOff()) current_state = STATE_FLASH_RL;
			break;
		case STATE_FLASH_RL: //If button 3 is pressed again, go to off state
			if(buttons[3].state == PRESSED) current_state = STATE_OFF_P;
			break;
		case STATE_DELAY_ADJ_P://If all butons are off, go to next state
			if(Button_allOff()) current_state = STATE_DELAY_ADJ;
			break;
		case STATE_DELAY_ADJ: //If buttons 0 or 3 are pressed, go to off state
			if(buttons[0].state == PRESSED) current_state = STATE_OFF_P;
			else if(buttons[3].state == PRESSED) current_state = STATE_OFF_P;
			break;
	}
}

void App_PerformActions()
{
	switch(current_state)
	{
		case STATE_OFF_P:
		case STATE_OFF:
			LED_Display = 0x00;
			break;
		case STATE_FLASH_LR_P:
		case STATE_FLASH_LR:
			flashLR();
			break;
		case STATE_FLASH_RL_P:
		case STATE_FLASH_RL:
			flashRL();
			break;
		case STATE_DELAY_ADJ_P:
		case STATE_DELAY_ADJ:
			delayAdj();
			break;
	}
}

void flashLR()
{
	if(timeCounter >= LED_Delay_ms)//If time elapsed is greater than the LED time increment
	{
		LED_State++; //Increment LED state
		timeCounter = 0; //Reset time counter
		LED_Display = ((LED_Display<<1)|0x01); //Increment display
		if(LED_State>4) //If LED state is greater than 4, reset LED state and display
		{
			LED_State = 0;
			LED_Display = 0x00;
		}
	}
}

void flashRL()
{
	if(timeCounter >= LED_Delay_ms)//If time elapsed is greater than the LED time increment
	{
		LED_State++;//Increment LED state
		timeCounter = 0;//Reset time counter
		LED_Display = ((LED_Display>>1)|0x08);//Increment display
		if(LED_State>4)//If LED state is greater than 4, reset LED state and display
		{
			LED_State = 0;
			LED_Display = 0x00;
		}
	}
}

void delayAdj()
{
	switch(delay_state)
	{
		case DELAY_STATE_WAIT_P: //If all buttons are off go to next state
			if(Button_allOff()) delay_state = DELAY_STATE_WAIT;
			break;
		case DELAY_STATE_WAIT:
			if(buttons[1].state == PRESSED) //If button 1 is pressed, Increment 
			{
				delay_state = DELAY_STATE_WAIT_P; //Update state
				if(LED_speed < 15)//check speed does not exceed bounds
				{//Increment speed
					LED_Delay_ms += LED_DELAY_INC;
					LED_speed ++;
				}
			}
			else if(buttons[2].state == PRESSED)  //If button 2 is pressed, Decrement
			{
				delay_state = DELAY_STATE_WAIT_P;//Update state
				if(LED_speed > 0)//check speed does not exceed bounds
				{//Decrement speed
					LED_Delay_ms -= LED_DELAY_INC;
					LED_speed --;
				}
			}
			break;
	}
	LED_Display = LED_speed;//update LED display
}	

void Display_Update()
{
	P2 = (0x0F|(~LED_Display)<<4);//Set P2 (~ is so LED_Diplay is active high)
}
