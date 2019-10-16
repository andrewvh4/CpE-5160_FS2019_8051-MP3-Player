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
	uint8_t rec_array[5];
	uint8_t uart_rec = 0;
 	uint8_t index = 0;
	uint8_t i;
	uint16_t block_num = 0;
	uint8_t multiplier = 1;
	
	printf("Enter Number for Block to Read: ");
	do
	{
		uart_rec = UART_Receive();
		if((uart_rec >= '0')&&(uart_rec <= '9'))
		{
			rec_array[index] = uart_rec-'0';
			index ++;
		}	
	}while(((uart_rec != '\n')||(uart_rec != '\r')) && index<5);
	
	for(i = index; i>0; i--) 
	{
		block_num = rec_array[i] * multiplier;
		multiplier*=10;
	}
	printf("\nBlock Number: %2.2bX\n", block_num);
	
	SD_readBlock(block_num, 512, array_out);

	print_memory(array_out, 512);
	
	Timing_delay_ms(1000);
	
	return(0);
}

