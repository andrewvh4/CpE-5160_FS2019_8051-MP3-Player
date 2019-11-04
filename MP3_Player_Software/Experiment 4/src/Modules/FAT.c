#include "SDCard.h"
#include "FAT.h"
#include "../Drivers/Port.h"
#include "../Drivers/SPI.h"
#include "../Drivers/Timing.h"
#include "../Main.h"
#include "../Drivers/UtPorts.h"
#include "../Drivers/Directory_Functions_globals.h"

uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t* array_for_data)
{
	uint8_t SDtype;
    uint8_t error_flag = FAT_NO_ERROR;

    SDtype = SD_getType(); // HC = 0, no change to sec # // SC = 9, sec # * 512 to convert to byte addr.
    Clear_bit_P1(nCS0);
    error_flag = SD_sendCommand(17, (sector_number << SDtype));

    if (error_flag == FAT_NO_ERROR)
    { 
        error_flag = SD_readBlock(sector_number, sector_size, array_for_data); 
    }

    Set_bit_P1(nCS0);

    if (error_flag != FAT_NO_ERROR)
    {
        error_flag = FAT_READ_SECTOR_ERROR;
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

uint8_t FAT_read16(uint16_t offset, uint8_t* array_name)
{
	uint16_t return_value = 0;
    uint8_t temp_value = 0;

    offset &= 0x1FF; // 0x1FF = 0d511 (Limit offset 0 - 511)
    return_value = *(array_name + offset + 1);
    return_value = return_value << BYTE;

    temp_value = *(array_name + offset);
    return_value |= temp_value;
    
    return return_value;
}

uint8_t FAT_read32(uint16_t offset, uint8_t* array_name)
{
	uint32_t return_value = 0;
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
	uint32_t TotalSectors, DataSectors, NumClusters, MBR_RelativeSectors, FATSize, RootCluster;
    uint16_t RSVDSectorCount, RootEntryCount, temp16;
    uint8_t NumFAT, temp8;
    uint8_t* input_array;

    input_array = array_in;

    printf("Locating Boot Sector...\n\r");

    Read_Sector(0, 512, input_array);
    temp8 = FAT_read8(0, input_array);

    if((temp8 != 0xEB) && (temp8 != 0xE9))
    {
      MBR_RelativeSectors = FAT_read32(MBR_RELATIVE_SECTORS, input_array);
   	  Read_Sector(MBR_RelativeSectors, 512, input_array);
	  printf("Relative Sectors = %ld\n\r", MBR_RelativeSectors);
	  temp8 = FAT_read8(0, input_array);
    }

    if((temp8 != 0xEB) && (temp8 != 0xE9))
    {
        printf("Disk Error!\n\r");
        return FAT_READ_SECTOR_ERROR;
    }

    gBytesPerSector = FAT_read16(BPB_BYTES_PER_SECTOR, input_array);
    gBytesPerSectorShift = 0;
    temp16 = gBytesPerSector;

    while(temp16 != 0x01)
    {
        gBytesPerSectorShift++;
        temp16 = temp16 >> 1;
    }

    gSectorsPerCluster = FAT_read8(BPB_SECTORS_PER_CLUSTER, input_array);
    RSVDSectorCount = FAT_read16(BPB_RSVD_SECTOR_COUNT, input_array);
    NumFAT = FAT_read8(BPB_NUM_FATS, input_array);
    RootEntryCount = FAT_read16(BPB_ROOT_ENTRY_COUNT, input_array);
    TotalSectors = FAT_read16(BPB_TOTAL_SECTORS_16, input_array);

    if(TotalSectors == 0) 
    {
        TotalSectors = FAT_read32(BPB_TOTAL_SECTORS_32, input_array);
    }

    FATSize = FAT_read16(BPB_FAT_SIZE_16, input_array);

    if(FATSize == 0)
    {
        FATSize = FAT_read32(BPB_FAT_SIZE_32, input_array);
        RootCluster = FAT_read32(BPB_ROOT_CLUSTER, input_array);
    }

    gRootDirectorySectors = ((RootEntryCount * 32) + (gBytesPerCluster - 1)) / gBytesPerSector;
    DataSectors = TotalSectors - (RSVDSectorCount + (NumFAT * FATSize) + gRootDirectorySectors);
    NumClusters = DataSectors / gSectorsPerCluster;
    gStartFAT = RSVDSectorCount + MBR_RelativeSectors;
    gFirstDataSector = gStartFAT + (NumFAT * FATSize) + gRootDirectorySectors;

    if(NumClusters < 65525)
    {
        gFATType = FAT16;
        gFATShift = FAT16_SHIFT;
        printf("FAT16 Detected...\n\r");
        gFirstRootDirectorySector = gStartFAT + (NumFAT * FATSize);

    }
    else
    {
        gFATType = FAT32;
        gFATShift = FAT32_SHIFT;
        printf("FAT32 Detected...\n\r");
        gFirstRootDirectorySector = ((RootCluster - 2) * gSectorsPerCluster) + gFirstDataSector;
    }
    
    return FAT_NO_ERROR;
}

uint32_t FAT_getFirstSector(uint32_t cluster_number)
{
    uint32_t sector_number;

    if(cluster_number == 0) 
    {
        sector_number = gFirstRootDirectorySector;
    }
    else
    {
        sector_number = ((cluster_number - 2) * gSectorsPerCluster) + gFirstDataSector;
    }

    return sector_number;
}

uint32_t FAT_getNextCluster(uint32_t cluster_number, uint8_t* array)
{
    uint32_t sector, return_cluster;
    uint16_t FATOffset;
    uint8_t* input_array;

    input_array = array;

    sector = (cluster_number >> (gBytesPerSectorShift - gFATShift)) + gStartFAT;
    Read_Sector(sector, gBytesPerSector, input_array);
    FATOffset = (uint16_t)((cluster_number << gFATShift) & (gBytesPerSector - 1));

    if (gFATType == FAT32) // For FAT32
    {
        return_cluster = (FAT_read32(FATOffset, input_array) & 0x0FFFFFFF);
    }
    else if (gFATType == FAT16) // For FAT16
    {
        return_cluster = (uint32_t)(FAT_read16(FATOffset, input_array));
        if (return_cluster == 0x0000FFFF) 
        {
            return_cluster = 0x0FFFFFFF;
        }
    }

    return return_cluster;
}