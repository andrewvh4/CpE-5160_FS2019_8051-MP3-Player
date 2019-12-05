#ifndef _OSFUNCTIONS_H
#define _OSFUNCITONS_H

#include "Main.h"
#include "Port.h"
#include <stdio.h>
#include "UART.h"
#include "OS.h"
#include "Buttons.h"

//State_P is used when the system is in that state but a button is pressed
//This is to prevent the system from changing states multiple times when a button is held

#define STATE_OFF_P 1
#define STATE_OFF 2
#define STATE_FLASH_LR_P 3
#define STATE_FLASH_LR 4
#define STATE_FLASH_RL_P 5
#define STATE_FLASH_RL 6
#define STATE_DELAY_ADJ_P 7
#define STATE_DELAY_ADJ 8

//These states are used in the delay adj state
#define DELAY_STATE_WAIT_P 1
#define DELAY_STATE_WAIT 2

//Sets increment values
#define LED_DELAY_INC 60
#define LED_MAX_VALUE 900
#define LED_MIN_VALUE LED_MAX_VALUE-15*LED_DELAY_INC


void App_UpdateState();
void App_PerformActions();
void Display_Update();

#endif
