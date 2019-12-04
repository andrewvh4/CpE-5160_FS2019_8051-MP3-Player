#ifndef _OSFUNCTIONS_H
#define _OSFUNCITONS_H

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


void App_UpdateState();
void App_PerformActions();
void Display_Update();

#endif
