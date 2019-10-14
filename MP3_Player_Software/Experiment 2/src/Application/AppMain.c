#include "../Main.h"
#include "../Drivers/Port.h"
#include <stdio.h>
#include "../Drivers/UART.h"
#include "../Drivers/Timing.h"

#include "../Drivers/memory_test.h"
#include "print_bytes.h"
#include "../Modules/LCD.h"
#include "../Drivers/Memory.h"
#include "../Drivers/SPI.h"

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
	Port_writePin(YELLOW_LED, LOW);
	
	Port_writeBus(PORT_1, 0xFF, 0xFF);
	
	setXRAM(XRAM_SIZE_1024);
	UART_Init_9600();
	//printf("U\n");
	if(SPI_ERROR_CLOCKRATE == SPI_Init(400000)) //Setup SPI with clockrate 400k
	{
		//printf("SPIIE\n");
	}
	else
	{
		//printf("I\n");
	}

	printf("SD Init:%h",SD_Init());

	//Setup SD
	//Posibly set up SPI with clockrate of 25M
	Timing_delay_ms(100);
	Port_writePin(GREEN_LED, HIGH);
	return(0);
}

uint8_t loop()
{
	//Prompt user to enter a block number to be read
	//Read a block
	//Print memory
	uint8_t return_value;
	uint8_t error_code;
	static uint8_t counter = 30;
	
	
	printf("\nLoop:%d", counter);

	Timing_delay_ms(1000);

	SPI_setCSState(LOW);
	
	error_code = SPI_Transfer(counter, &return_value);
	counter = counter +1;
	
	if(error_code == SPI_NO_ERROR)
	{
		//printf("T\n");
	}
	
	SPI_setCSState(HIGH);
	return(0);
}

