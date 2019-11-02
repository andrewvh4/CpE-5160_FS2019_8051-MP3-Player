#include "SDCard.h"
#include "FAT.h"
#include "../Drivers/Port.h"
#include "../Drivers/SPI.h"
#include "../Drivers/Timing.h"
#include "../Main.h"

uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data)
{
	
}

uint8_t FAT_read8(uint16_t offset, uint8_t * array_name)
{
	
}

uint8_t FAT_read16(uint16_t offset, uint8_t * array_name)
{
	
}

uint8_t FAT_read8(uint16_t offset, uint8_t * array_name)
{
	
}


uint8_t FAT_mountDrive(void)
{
	
}

uint32_t FAT_getFirstSector(uint32_t cluser_number)
{
	
}

uint32_t FAT_getNextCluster(uint32_t cluster_number, uint8_t * array)
{
	
}