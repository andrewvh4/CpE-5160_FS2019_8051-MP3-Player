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
uint8_t loop_debug();


void main()
{
	setup();
	while(1)
	{
		loop_debug();
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
	SPI_ERROR_CLOCKRATE == SPI_Init(400000);
	
	//Init STA013
	
	printf("SD Init:%2.2bX\n",SD_Init());

	//Setup SD
	//Posibly set up SPI with clockrate of 25M
	Timing_delay_ms(100);
	Port_writePin(GREEN_LED, HIGH);
	return(0);
}

uint8_t loop_debug()
{
	//Verify Functionality of I2C by reading first three STA013 addresses on startup
	
	return(0);
}

uint8_t loop()
{
	
}