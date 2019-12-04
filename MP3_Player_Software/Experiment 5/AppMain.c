#include "Main.h"
#include "Port.h"
#include <stdio.h>
#include "UART.h"
#include "OS.h"
#include "Buttons.h"

uint8_t setup();

void main()
{
	setup();
	while(1);
}

uint8_t setup()
{
	uint8_t initError;

	P2 = 0x0F;
	Port_writePin(GREEN_LED, LOW);
	
	Port_writeBus(PORT_1, 0xFF, 0xFF);
	initError |= UART_Init_9600();

	Button_initParams();

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
