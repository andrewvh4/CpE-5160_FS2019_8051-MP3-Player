#include "../main.h"
#include "../Drivers/Port.h"
#include <stdio.h>
#include "../Drivers/UART.h"
#include "../Drivers/Timing.h"

#include "../Drivers/memory_test.h"
#include "print_bytes.h"

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
	Port_writePin(GREEN_LED, LOW);
	
	
	//Serial Init
	UART_Init_9600();
	
	Timing_delay_ms(1000);
	Port_writePin(GREEN_LED, HIGH);
	return(1);
}

uint8_t loop()
{
	uint8_t* generic_pointer;
	
	generic_pointer = xdata_memory_init();
	
	print_memory(generic_pointer, 100);
	
	Timing_delay_ms(1000);
	return(1);
}

