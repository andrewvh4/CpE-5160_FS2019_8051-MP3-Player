#include "../main.h"
#include "../Drivers/Port.h"
#include <stdio.h>
#include "../Drivers/UART.h"
#include "../Drivers/Timing.h"

uint8_t setup();
uint8_t loop();


void main()
{
	setup();
	while(1)
	{
		loop();
	}
}

uint8_t setup()
{
	//Serial Init
	Port_writePin(SW1, HIGH);
	Port_writePin(GREENLED, HIGH);
	return(1);
}

uint8_t loop()
{
	while(Port_readPin(SW1))
	{
		Port_writePin(GREENLED, LOW);
		Timing_delay_ms(1000);
		Port_writePin(GREENLED, HIGH);
		Timing_delay_ms(1000);
	}
	
	return(1);
}

