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
#include "../Modules/SDCard.h"
#include "Long_Serial_In.h"

uint8_t xdata array_out[512];

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

	printf("SD Init:%2.2bX\n",SD_Init());

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
	uint32_t block_num = 0;
	
	printf("Enter a Block number: 0x");
	block_num = long_serial_input();

	printf("Block Number: %8.8lX\n", block_num);
	
	SD_readBlock(block_num, 512, array_out);

	print_memory(array_out, 512);
	
	return(0);
}

