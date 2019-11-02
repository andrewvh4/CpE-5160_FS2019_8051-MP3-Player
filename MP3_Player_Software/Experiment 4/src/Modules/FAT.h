/*------------------------------------------------------------------*-

   SDCard.h (v1.00)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-10-01

   Description: Defines functions SD Card operation
-*------------------------------------------------------------------*/
#include "../Main.h"
#ifndef _FAT_H
#define _FAT_H

// ------ Public function prototypes -------------------------------
uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data);

uint8_t FAT_read8(uint16_t offset, uint8_t * array_name);
uint8_t FAT_read16(uint16_t offset, uint8_t * array_name);
uint8_t FAT_read32(uint16_t offset, uint8_t * array_name);

uint8_t FAT_mountDrive(void);
uint32_t FAT_getFirstSector(uint32_t cluser_number);
uint32_t FAT_getNextCluster(uint32_t cluster_number, uint8_t * array);


#define FAT_NO_ERROR 0
#define FAT_READ_DECTOR_ERROR 1

#endif