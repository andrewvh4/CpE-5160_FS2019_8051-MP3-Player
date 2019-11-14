/*--------------------------------------------------------------------

   SDCard.h (v1.01)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-10-01

   Description: Defines functions SD Card operation
--------------------------------------------------------------------*/

#ifndef _FAT_H
#define _FAT_H

#include "../Main.h"
#include "../Drivers/Directory_Functions_globals.h"

//------- Public Constants  -----------------------------------------

#define FAT32 4
#define FAT16 2
#define FAT32_SHIFT 2
#define FAT16_SHIFT 1

#define FAT_NO_ERROR 0
#define FAT_READ_SECTOR_ERROR 1

#define FAT_MORE_ENTRIES (0x8000)   
#define FAT_NO_ENTIRES_FOUND (0x80000000)  // msb set to indicate error
#define FAT_DIRECTORY_BIT  (0x10000000)  // lsb of first nibble (bit28)

#define MBR_RELATIVE_SECTORS (0x01C6)
#define BPB_BYTES_PER_SECTOR (0x0B)
#define BPB_SECTORS_PER_CLUSTER (0x0D)
#define BPB_RSVD_SECTOR_COUNT (0x0E)
#define BPB_NUM_FATS (0x10)
#define BPB_ROOT_ENTRY_COUNT (0x11)
#define BPB_TOTAL_SECTORS_16 (0x13)
#define BPB_FAT_SIZE_16 (0x16)
#define BPB_TOTAL_SECTORS_32 (0x20)
#define BPB_FAT_SIZE_32 (0x24)
#define BPB_ROOT_CLUSTER (0x2C)

// ------ Public function prototypes -------------------------------
uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t* array_for_data);

uint8_t FAT_read8(uint16_t offset, uint8_t* array_name);
uint16_t FAT_read16(uint16_t offset, uint8_t* array_name);
uint32_t FAT_read32(uint16_t offset, uint8_t* array_name);

uint8_t FAT_mountDrive(uint8_t* array_in);
uint32_t FAT_getFirstSector(uint32_t cluster_number);
uint32_t FAT_getNextCluster(uint32_t cluster_number, uint8_t* array);

#endif