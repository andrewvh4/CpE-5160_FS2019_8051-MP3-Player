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
	UART_Init_9600();
	
	Port_writePin(GREEN_LED, LOW);
	Timing_delay_ms(1000);
	Port_writePin(GREEN_LED, HIGH);
	
	return(1);
}

uint8_t loop()
{
	printf("Hello World\n");
	Timing_delay_ms(1000);
	
	return(1);
}

