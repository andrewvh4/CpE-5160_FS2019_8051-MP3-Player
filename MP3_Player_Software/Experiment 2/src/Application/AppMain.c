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
	//Set XRAM Size 1024
	//Setup UART
	//Setup SPI with clockrate 400k
	//Setup SD
	//Posibly set up PSI with clockrate of 25M
	
	return(0);
}

uint8_t loop()
{
	//Prompt user to enter a block number to be read
	//Read a block
	//Print memory

	return(0);
}

