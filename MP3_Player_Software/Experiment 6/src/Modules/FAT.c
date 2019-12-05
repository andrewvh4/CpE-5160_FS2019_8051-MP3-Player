#include "SDCard.h"
#include "FAT.h"
#include "../Drivers/Port.h"
#include "../Drivers/SPI.h"
#include "../Drivers/Timing.h"
#include "../Main.h"
#include "../Drivers/UtPorts.h"
#include "../Drivers/Directory_Functions_globals.h"
#include "../Application/print_bytes.h"

extern uint32_t idata FirstDataSec_g, StartofFAT_g, FirstRootDirSec_g, RootDirSecs_g;
extern uint16_t idata BytesPerSec_g;
extern uint8_t idata SecPerClus_g, FATtype_g, BytesPerSecShift_g,FATshift_g;

uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t* array_for_data)
{
    uint8_t idata error_flag = FAT_NO_ERROR;

    Clear_bit_P1(nCS0);
	//Send command 17
    error_flag = SD_sendCommand(17, sector_number);

	//Read block if no errors
    if (error_flag == FAT_NO_ERROR)
    { 
        error_flag = SD_readBlock(sector_number, sector_size, array_for_data); 
    }

    Set_bit_P1(nCS0);

    if (error_flag != FAT_NO_ERROR) //Try again if you get an error
    {
		//printf("Attempting Second Read\n\r");
		Clear_bit_P1(nCS0);
	    error_flag = SD_sendCommand(17, sector_number);
	
	    if (error_flag == FAT_NO_ERROR)
	    { 
	        error_flag = SD_readBlock(sector_number, sector_size, array_for_data); 
	    }
	
	    Set_bit_P1(nCS0);
	
	    if (error_flag != FAT_NO_ERROR)
	    {
	        error_flag = FAT_READ_SECTOR_ERROR;
	    }
	}

    return error_flag;
}

uint8_t FAT_read8(uint16_t offset, uint8_t* array_name)
{
	uint8_t return_value;

    offset &= 0x1FF; // 0x1FF = 0d511 (Limit offset 0 - 511)
    return_value = *(array_name + offset);

    return return_value;
}

uint16_t FAT_read16(uint16_t offset, uint8_t* array_name)
{
	uint16_t idata return_value = 0;
    uint8_t temp_value = 0;

    offset &= 0x1FF; // 0x1FF = 0d511 (Limit offset 0 - 511)
    return_value = *(array_name + offset + 1);
    return_value = return_value << BYTE;

    temp_value = *(array_name + offset);
    return_value |= temp_value;
    
    return return_value;
}

uint32_t FAT_read32(uint16_t offset, uint8_t* array_name)
{
	uint32_t idata return_value = 0;
    uint8_t temp_value = 0;
    uint8_t index = 0;

    offset &= 0x1FF; // 0x1FF = 0d511 (Limit offset 0 - 511)

    for(index = 0; index < 4; index++)
    {
        temp_value = *(array_name + offset + (3 - index));
        return_value = return_value << BYTE;
        return_value |= temp_value;
    }

    return return_value;
}

uint8_t FAT_mountDrive(uint8_t* array_in)
{
	uint32_t idata TotalSectors, DataSectors, NumClusters, MBR_RelativeSectors, FATSize, RootCluster;
    uint16_t idata RSVDSectorCount, RootEntryCount, temp16;
    uint8_t NumFAT, temp8;
    uint8_t* input_array;

    input_array = array_in;

	printf("---Mounting SD Card\n\r");
    printf("Locating Boot Sector...\n\r");

    Read_Sector(0, 512, input_array);
    temp8 = FAT_read8(0, input_array);

    if((temp8 != 0xEB) && (temp8 != 0xE9))
    {
      MBR_RelativeSectors = FAT_read32(MBR_RELATIVE_SECTORS, input_array);
   	  printf("Relative Sectors = %08lX\n\r", MBR_RelativeSectors);
	  Read_Sector(MBR_RelativeSectors, 512, input_array);
	  temp8 = FAT_read8(0, input_array);
    }

    if((temp8 != 0xEB) && (temp8 != 0xE9))
    {
        printf("Disk Error!\n\r");
        return FAT_READ_SECTOR_ERROR;
    }

    BytesPerSec_g = FAT_read16(BPB_BYTES_PER_SECTOR, input_array);
    BytesPerSecShift_g = 0;
    temp16 = BytesPerSec_g;

	printf("Bytes per Sector: %hX\n\r", BytesPerSec_g);

    while(temp16 != 0x01)
    {
        BytesPerSecShift_g++;
        temp16 = temp16 >> 1;
    }

    SecPerClus_g = FAT_read8(BPB_SECTORS_PER_CLUSTER, input_array);
    RSVDSectorCount = FAT_read16(BPB_RSVD_SECTOR_COUNT, input_array);
    NumFAT = FAT_read8(BPB_NUM_FATS, input_array);
    RootEntryCount = FAT_read16(BPB_ROOT_ENTRY_COUNT, input_array);
    TotalSectors = FAT_read16(BPB_TOTAL_SECTORS_16, input_array);
	
	printf("Sectors per Cluster: %2.2bX\n\r", SecPerClus_g);
	printf("Reserved Sector Count: %hX\n\r", RSVDSectorCount);
	printf("Number of FAT: %2.2bX\n\r", NumFAT);
	printf("Root Entry Count: %hX\n\r", RootEntryCount);
	printf("Total Sectors Pre: %lX\n\r", TotalSectors);

    if(TotalSectors == 0) 
    {
        TotalSectors = FAT_read32(BPB_TOTAL_SECTORS_32, input_array);
		printf("Total Sectors: %lX\n\r", TotalSectors);
    }

    FATSize = FAT_read16(BPB_FAT_SIZE_16, input_array);
	printf("FAT Size Pre: %lX\n\r", FATSize);

    if(FATSize == 0)
    {
        FATSize = FAT_read32(BPB_FAT_SIZE_32, input_array);
        RootCluster = FAT_read32(BPB_ROOT_CLUSTER, input_array);
		printf("FAT Size: %lX\n\r", FATSize);
		printf("Root Cluster: %lX\n\r", RootCluster);
    }

    RootDirSecs_g = ((RootEntryCount * 32) + (BytesPerSec_g - 1)) / BytesPerSec_g;
    DataSectors = TotalSectors - (RSVDSectorCount + (NumFAT * FATSize) + RootDirSecs_g);
    NumClusters = DataSectors / SecPerClus_g;
    StartofFAT_g = RSVDSectorCount + MBR_RelativeSectors;
    FirstDataSec_g = StartofFAT_g + (NumFAT * FATSize) + RootDirSecs_g;

	printf("Root Directory Sectors: %lX\n\r", RootDirSecs_g);
	printf("Data Sectors: %lX\n\r", DataSectors);
	printf("Number of Clusters: %lX\n\r", NumClusters);
	printf("Start of FAT: %lX\n\r", StartofFAT_g);
	printf("First Data Sector: %lX\n\r", FirstDataSec_g);
    if(NumClusters < 65525)
    {
        FATtype_g = FAT16;
        FATshift_g = FAT16_SHIFT;
        printf("FAT16 Detected...\n\r");
        FirstRootDirSec_g = StartofFAT_g + (NumFAT * FATSize);

    }
    else
    {
        FATtype_g = FAT32;
        FATshift_g = FAT32_SHIFT;
        printf("FAT32 Detected...\n\r");
        FirstRootDirSec_g = ((RootCluster - 2) * SecPerClus_g) + FirstDataSec_g;
    }
    
    return FAT_NO_ERROR;
}

uint32_t FAT_getFirstSector(uint32_t cluster_number)
{
    uint32_t idata sector_number;

    if(cluster_number == 0) 
    {
        sector_number = FirstRootDirSec_g;
    }
    else
    {
		//Calculate first sector of cluster
        sector_number = ((cluster_number - 2) * SecPerClus_g) + FirstDataSec_g;
    }

    return sector_number;
}

uint32_t FAT_getNextCluster(uint32_t cluster_number, uint8_t* array)
{
    uint32_t idata sector, return_cluster;
    uint16_t idata FATOffset;
    uint8_t* input_array;

    input_array = array;

    sector = (cluster_number >> (BytesPerSecShift_g - FATshift_g)) + StartofFAT_g;
    Read_Sector(sector, BytesPerSec_g, input_array);
    FATOffset = (uint16_t)((cluster_number << FATshift_g) & (BytesPerSec_g - 1));

    if (FATtype_g == FAT32) // For FAT32
    {
        return_cluster = (FAT_read32(FATOffset, input_array) & 0x0FFFFFFF);
    }
    else if (FATtype_g == FAT16) // For FAT16
    {
        return_cluster = (uint32_t)(FAT_read16(FATOffset, input_array));
        if (return_cluster == 0x0000FFFF) 
        {
            return_cluster = 0x0FFFFFFF;
        }
    }

    return return_cluster;
}