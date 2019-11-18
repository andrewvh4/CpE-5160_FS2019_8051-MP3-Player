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

extern uint32_t idata FirstDataSec_g, StartofFAT_g, FirstRootDirSec_g, RootDirSecs_g;
extern uint16_t idata BytesPerSec_g;
extern uint8_t idata SecPerClus_g, FATtype_g, BytesPerSecShift_g,FATshift_g;

uint16_t entries;
uint32_t current_directory_sector;

uint8_t setup();
uint8_t loop();
void printFile(uint32_t startingCluster);


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
	
	//initialise SD card
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

	printf("---Start of Program---\n\r");

	current_directory_sector = FirstRootDirSec_g;
	
	Timing_delay_ms(100);
	Port_writePin(GREEN_LED, HIGH);
	return(0);
}

uint8_t loop()
{
	uint8_t temp_character;
	uint8_t entry_number;
	uint32_t cluster_number;

	//Print the current directory
	printf("Current Directory Sector:%08lX\n\r", current_directory_sector);
	entries = Print_Directory(current_directory_sector, buffer_1);

	//Read in a Character
	printf("Enter a Number\n\r");
	temp_character = UART_Receive();
	printf("You Entered: %d\m\r", temp_character);
	entry_number = temp_character - '0';

	//Check for Invalid Entries
	if(entry_number>entries)
	{
		printf("Invalid Entry Number\n\r");
	}
	else
	{
		//Read directory entry
		cluster_number = Read_Dir_Entry(current_directory_sector, entry_number, buffer_1);
		printf("Cluster Number: %08lX\n\r", cluster_number);
		//Check for Errors
		if((cluster_number & 0x80000000) == 0x80000000)
		{
			printf("An Error Occured\n\r");
		}
		else
		{
			//See if entry is a file or directory
			if((cluster_number & 0x10000000) == 0x10000000) 
			{
				//Go to the directory
				printf("You selected a Directory\n\r");
				current_directory_sector = FAT_getFirstSector(cluster_number & 0x0FFFFFFF);
			}
			else
			{
				//Open the file
				printf("You selected a File\n\r");
				printFile(cluster_number & 0x0FFFFFFF);
			}
		}
	}
	return(0);
}

void printFile(uint32_t startingCluster)
{
	uint32_t current_sector;
	uint32_t current_cluster;
	uint8_t temp_character;
	uint8_t sector_in_cluster = 0;

	//Get starting parameters of file
	current_sector = FAT_getFirstSector(startingCluster);
	current_cluster = startingCluster;

	do
	{
		//Print the file
		printf("Sector:%08lX\n\rCluster:%08lX\n\r", current_sector, current_cluster);
		Read_Sector(current_sector, 512, buffer_1);
		print_memory(buffer_1, 512);
	
		//Get user input
		printf("Continue(c)\n\rExit(x)\n\r");
		do
		{
			temp_character = UART_Receive();
		} while((temp_character!='x')&&(temp_character!='X')&&
				(temp_character!='c')&&(temp_character!='C'));
		
		//Go to next sector
		if(sector_in_cluster >= SecPerClus_g-1)
		{
			//Go to next cluster
			printf("Next Cluster\n\r");
			current_cluster = FAT_getNextCluster(current_cluster, buffer_1);
			current_sector = FAT_getFirstSector(current_cluster);
			sector_in_cluster = 0;
		}
		else
		{
			//increment Sector
			current_sector++;
			sector_in_cluster++;
		}
		 

	} while((temp_character!='x')&&(temp_character!='X'));
	return;
}

