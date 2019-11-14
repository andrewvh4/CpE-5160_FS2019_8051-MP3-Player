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

uint8_t xdata buffer_1[512];
//uint8_t xdata buffer_2[512];

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
	uint8_t initError = 0;
	uint16_t index = 0;

	Port_writePin(GREEN_LED, LOW);
	
	Port_writeBus(PORT_1, 0xFF, 0xFF);
	initError |= UART_Init_9600();

	setXRAM(XRAM_SIZE_1024);
	
	initError |= SPI_Init(400000);
	initError |= SD_Init();
	
	for (index = 0; index < 512; index++)
	{
		buffer_1[index] = 0xFF;
	}

	initError |= FAT_mountDrive(buffer_1);
	
	initError |= STA013_Init();
	printf("Error: %d\n", initError);
	
	if (!initError)
	{
		printf("*Setup Complete*\n\n");
	}
	else
	{
		printf("*Setup Failed*\n\n");
	}

	
	Timing_delay_ms(100);
	Port_writePin(GREEN_LED, HIGH);
	return(0);
}

uint8_t loop()
{
	// uint32_t gRootDirectorySectors;
	// uint32_t gStartFAT;
	// uint32_t gFirstDataSector;
	// uint32_t gFirstRootDirectorySector;
	
	// uint16_t gBytesPerSector;
	// uint16_t gBytesPerCluster;
	
	// uint8_t gBytesPerSectorShift;
	// uint8_t gSectorsPerCluster;
	// uint8_t gFATType;
	// uint8_t gFATShift;

	// Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t* array_for_data);

	// FAT_read8(uint16_t offset, uint8_t* array_name);
	// FAT_read16(uint16_t offset, uint8_t* array_name);
	// FAT_read32(uint16_t offset, uint8_t* array_name);
	
	// FAT_mountDrive(uint8_t* array_in);
	// FAT_getFirstSector(uint32_t cluster_number);
	// FAT_getNextCluster(uint32_t cluster_number, uint8_t* array);
	int i = 0;
	for(i = 0; i <= 100; i++)
	{
		Timing_delay_ms(100);
		//printf("Test: %d\n", i);
	}
	return 0x00;
}
