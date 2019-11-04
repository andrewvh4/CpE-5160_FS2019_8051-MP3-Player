#include "../Main.h"
#include "../Drivers/Port.h"
#include <stdio.h>
#include "../Drivers/UART.h"
#include "../Drivers/Timing.h"

#include "../Drivers/memory_test.h"
#include "print_bytes.h"
#include "../Modules/LCD.h"
#include "../Modules/FAT.h"
#include "../Modules/SDCard.h"
#include "../Modules/STA013.h"
#include "../Drivers/Memory.h"
#include "../Drivers/SPI.h"
#include "../Drivers/Directory_Functions_globals.h"

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
	
	Port_writeBus(PORT_1, 0xFF, 0xFF);
	UART_Init_9600();
	
	setXRAM(XRAM_SIZE_1024);
	
	SPI_Init(400000);
	SD_Init();

	STA013_Init();

	printf("***Setup Complete***\n\n");
	Timing_delay_ms(100);
	Port_writePin(GREEN_LED, HIGH);
	return(0);
}

uint8_t loop()
{
	
	return(0);
}
