#include "Main.h"
#include "Port.h"
#include <stdio.h>
#include "UART.h"
#include "OS.h"
#include "Buttons.h"

#define STATE_OFF_P 1
#define STATE_OFF 2
#define STATE_FLASH_LR_P 3
#define STATE_FLASH_LR 4
#define STATE_FLASH_RL_P 5
#define STATE_FLASH_RL 6
#define STATE_DELAY_ADJ_P 7
#define STATE_DELAY_ADJ 8

#define DELAY_STATE_WAIT_P 1
#define DELAY_STATE_WAIT 2

#define LED_DELAY_INC 60
#define LED_MAX_VALUE 900
#define LED_MIN_VALUE LED_MAX_VALUE-15*LED_DELAY_INC

uint8_t LED_State = 0;
uint8_t LED_Display = 0x00;
uint8_t current_state = STATE_OFF;
uint16_t LED_Delay_ms = LED_MAX_VALUE;
uint8_t LED_speed = 15;
uint8_t timeCounter = 0;
uint8_t delay_state = 0;

uint8_t setup();
void flashLR();
void flashRL();
void delayAdj();

void main()
{
	setup();
	while(1);
}

uint8_t setup()
{
	uint8_t initError;


	Port_writePin(GREEN_LED, LOW);
	
	Port_writeBus(PORT_1, 0xFF, 0xFF);
	initError |= UART_Init_9600();

	sEOS_Init();
	
	if (!initError)
	{
		printf("*Setup Complete*\n\n");
	}
	else
	{
		printf("*Setup Failed*\n\n");
	}

	Port_writePin(GREEN_LED, HIGH);
	return(0);
}

void App_UpdateState()
{
	timeCounter += sEOS_LOOP_INTERVAL_ms;
	switch(current_state)
	{
		case STATE_OFF_P:
			if(Button_allOff()) current_state = STATE_OFF;
			LED_State = 0;
			LED_Display = 0x00;
			delay_state = DELAY_STATE_WAIT_P;
			break;
		case STATE_OFF:
			if(buttons[0].state == PRESSED) current_state = STATE_FLASH_LR_P;
			else if(buttons[1].state == PRESSED) current_state = STATE_DELAY_ADJ_P;
			else if(buttons[2].state == PRESSED) current_state = STATE_DELAY_ADJ_P;
			else if(buttons[3].state == PRESSED) current_state = STATE_FLASH_RL_P;
			break;
		case STATE_FLASH_LR_P:
			if(Button_allOff()) current_state = STATE_FLASH_LR;
			break;
		case STATE_FLASH_LR:
			if(buttons[0].state == PRESSED) current_state = STATE_OFF_P;
			break;
		case STATE_FLASH_RL_P:
			if(Button_allOff()) current_state = STATE_FLASH_RL;
			break;
		case STATE_FLASH_RL:
			if(buttons[3].state == PRESSED) current_state = STATE_OFF_P;
			break;
		case STATE_DELAY_ADJ_P:
			if(Button_allOff()) current_state = STATE_DELAY_ADJ;
			break;
		case STATE_DELAY_ADJ:
			if(buttons[0].state == PRESSED) current_state = STATE_OFF_P;
			else if(buttons[4].state == PRESSED) current_state = STATE_OFF_P;
			break;
	}
}

void App_PerformActions()
{
	switch(current_state)
	{
		case STATE_OFF_P:
		case STATE_OFF:
			if(buttons[0].state == PRESSED) current_state = STATE_FLASH_LR_P;
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
	if(timeCounter >= LED_Delay_ms)
	{
		timeCounter = 0;
		LED_Display = ((LED_Display<<1)|0x01);
		if(LED_State>4)
		{
			LED_State = 0;
			LED_Display = 0x00;
		}
		LED_State++;
	}
}

void flashRL()
{
	if(timeCounter >= LED_Delay_ms)
	{
		timeCounter = 0;
		LED_Display = ((LED_Display>>1)|0x80);
		if(LED_State>4)
		{
			LED_State = 0;
			LED_Display = 0x00;
		}
		LED_State++;
	}
}

void delayAdj()
{
	switch(delay_state)
	{
		case DELAY_STATE_WAIT_P:
			if(Button_allOff()) delay_state = DELAY_STATE_WAIT;
			break;
		case DELAY_STATE_WAIT:
			if(buttons[1].state == PRESSED) //Increment 
			{
				delay_state = DELAY_STATE_WAIT_P;
				if(LED_speed < 15)
				{
					LED_Delay_ms += LED_DELAY_INC;
					LED_speed ++;
					LED_Display = LED_speed;
				}
			}
			else if(buttons[2].state == PRESSED)  //Decrement
			{
				delay_state = DELAY_STATE_WAIT_P;
				if(LED_speed > 0)
				{
					LED_Delay_ms -= LED_DELAY_INC;
					LED_speed --;
					LED_Display = LED_speed;
				}
			}
			break;
	}
}	
