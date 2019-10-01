#include "../main.h"
#include "../Drivers/Port.h"
#include <stdio.h>
#include "../Drivers/UART.h"
#include "../Drivers/Timing.h"

#include "../Drivers/memory_test.h"
#include "print_bytes.h"
#include "../Modules/LCD.h"

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
	//Set XRAM Size
	
	
	return(1);
}

uint8_t loop()
{
	

	return(1);
}

