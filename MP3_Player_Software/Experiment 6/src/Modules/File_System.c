#include "../AT89C51RC2.h"
#include <stdio.h>
#include "../main.h"
#include "../Drivers/Port.h"
#include "../Drivers/UART.h"
#include "../Drivers/SPI.h"
#include "../Modules/SDCard.h"
#include "File_System.h"
#include "../Application/print_bytes.h"
#include "Directory_Functions.h"
#include "Read_Sector.h"
#include "../Application/Play_Song.h"

extern uint32_t idata FirstDataSec_g, StartofFAT_g, FirstRootDirSec_g, RootDirSecs_g;
extern uint16_t idata BytesPerSec_g;
extern uint8_t idata SecPerClus_g, FATtype_g, BytesPerSecShift_g,FATshift_g;


// Read Value Functions

/***********************************************************************
DESC: Reads a specified byte from a block of memory 
INPUT: Offset to the byte, Starting address of the block of memory
RETURNS: uint8_t specifed byte
CAUTION: 
************************************************************************/
uint8_t read8(uint16_t offset, uint8_t * array_name)
{
   uint8_t return_val;
   offset&=0x1FF;  // limit offset to 0 to 511
   return_val=*(array_name+offset);
   return return_val;
}

/***********************************************************************
DESC: Reads a specified integer (stored little endian) from a block of memory 
INPUT: Offset to the LSB of the integer, Starting address of the block of memory
RETURNS: uint16_t specfied integer
CAUTION: 
************************************************************************/

uint16_t read16(uint16_t offset, uint8_t * array_name)
{
   uint16_t return_val,temp;
   offset&=0x1FF;  // limit offset to 0 to 511
   return_val=0;
   return_val=*(array_name+offset+1);
   return_val=return_val<<8;
   temp=*(array_name+offset);
   return_val|=temp;
   return return_val;
}

/***********************************************************************
DESC: Reads a specified word (stored little endian) from a block of memory 
INPUT: Offset to the LSB of the word, Starting address of the block of memory
RETURNS: uint32_t specfied word
CAUTION: 
************************************************************************/

uint32_t read32(uint16_t offset, uint8_t * array_name)
{
   uint32_t return_val;
   uint8_t temp, i;
   return_val=0;
   offset&=0x1FF;  // limit offset to 0 to 511
   for(i=0;i<4;i++)
   {   
       temp=*(array_name+offset+(3-i));
       return_val=return_val<<8;
       return_val|=temp;
   }
   return return_val;
}

/***********************************************************************
DESC: Determines FAT type, Start of the FAT, Root Directory Sector
      and Start of Data for an SD card
INPUT: Start of a block of memory in xdata that can be used 
       to read blocks from the SD card
RETURNS: uint8_t error code    Creates globals in idata for: 
idata uint32_t FATSz, RelSec, RootClus, FirstDataSec_g, StartofFAT_g;
idata uint16_t BytesPerSec_g, BytesPerSecM, RootEntCnt;
idata uint8_t SecPerClus, FATtype, SecShift;

CAUTION: Does not work with FAT16 yet!
************************************************************************/

uint8_t Mount_Drive(uint8_t xdata * array_in)
{
  uint32_t TotSec, DataSec, CntClus,MBR_RelSec, FATSz, RootClus;
  uint16_t RsvdSecCnt, RootEntCnt, temp16;
  uint8_t NumFAT, temp8;
  uint8_t * values;

   

   values=array_in;
   printf("Locating Boot Sector...\n\r");
   Read_Sector(0, 512, values);
   temp8=read8(0,values);
   if((temp8!=0xEB)&&(temp8!=0xE9))
   {
      MBR_RelSec=read32(MBR_RelSectors,values);
	  Read_Sector(MBR_RelSec, 512, values);
	  printf("Relative Sectors = %ld\n\r",MBR_RelSec);
	  temp8=read8(0,values);
   }
   if((temp8!=0xEB)&&(temp8!=0xE9))
   {
      printf("Disk Error!\n\r");
	  return Disk_Error;
   }
   BytesPerSec_g=read16(BPB_BytesPerSector,values);
   BytesPerSecShift_g=0;
   temp16=BytesPerSec_g;
   while(temp16!=0x01)
   {
      BytesPerSecShift_g++;
      temp16=temp16>>1;
   }
   SecPerClus_g=read8(BPB_SectorsPerCluster,values);
   RsvdSecCnt=read16(BPB_RsvdSectorCount,values);
   NumFAT=read8(BPB_NumberFATs,values);
   RootEntCnt=read16(BPB_RootEntryCount,values);
   TotSec=read16(BPB_TotalSectors16,values);
   if(TotSec==0) TotSec=read32(BPB_TotalSectors32,values);
   FATSz=read16(BPB_FATsize16,values);
   if(FATSz==0)
   {
	  FATSz=read32(BPB_FATsize32,values);
      RootClus=read32(BPB_RootCluster,values);
   }
   RootDirSecs_g=((RootEntCnt * 32) + (BytesPerSec_g - 1))/BytesPerSec_g;
   DataSec=TotSec-(RsvdSecCnt+(NumFAT*FATSz)+RootDirSecs_g);
   CntClus=DataSec/SecPerClus_g;
   StartofFAT_g=RsvdSecCnt+MBR_RelSec;
   FirstDataSec_g=StartofFAT_g+(NumFAT*FATSz)+RootDirSecs_g;
   if(CntClus<65525)
   {
     FATtype_g=FAT16;
	 FATshift_g=FAT16_shift;
	 printf("FAT16 Detected...\n\r");
	 FirstRootDirSec_g=StartofFAT_g+(NumFAT*FATSz);

   }
   else
   {
     FATtype_g=FAT32;
	 FATshift_g=FAT32_shift;
	 printf("FAT32 Detected...\n\r");
     FirstRootDirSec_g=((RootClus-2)*SecPerClus_g)+FirstDataSec_g;
   }
   return No_Disk_Error;
}


/***********************************************************************
DESC: Calculates the First Sector of a given Cluster 
INPUT: uint32_t Cluster number
RETURNS: uint32_t sector number 
CAUTION: 
************************************************************************/
uint32_t First_Sector (uint32_t Cluster_num)
{
   uint32_t Sector_num;
   if(Cluster_num==0) 
   {
       Sector_num=FirstRootDirSec_g;
   }
   else
   {
       Sector_num=((Cluster_num-2)*SecPerClus_g)+FirstDataSec_g;
   }
   return Sector_num;
}




/***********************************************************************
DESC: Finds the next cluster number of a file in the FAT with an input
      parameter of the current cluster number.
INPUT: Current cluster number.
RETURNS: Next Cluster number from the FAT
CAUTION: 
************************************************************************/


uint32_t Find_Next_Clus(uint32_t Cluster_num, uint8_t xdata * array_name)
{
   uint32_t Sector, return_clus;
   uint16_t FAToffset;
   uint8_t * values;
   values=array_name;
   Sector=(Cluster_num>>(BytesPerSecShift_g-FATshift_g))+StartofFAT_g;
   Read_Sector(Sector, BytesPerSec_g, values);
   FAToffset=(uint16_t)((Cluster_num<<FATshift_g)&(BytesPerSec_g-1));
   if(FATtype_g==FAT32)    // FAT32
   {
       return_clus=(read32(FAToffset,values)&0x0FFFFFFF);
   }
   else if(FATtype_g==FAT16)    // FAT16
   {
       return_clus=(uint32_t)(read16(FAToffset,values));
       if(return_clus==0x0000FFFF) return_clus=0x0FFFFFFF;
   }
   return return_clus;
}
   



/***********************************************************************
DESC: Prints the sectors of a file until the user hits X
      Prints the Cluster Number and Sector number of each sector as well
INPUT: Starting Cluster of the file and the start of a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint8_t 0
CAUTION: 
************************************************************************/


uint8_t Open_File(uint32_t Cluster_num, uint8_t xdata * array_in)
{
   uint32_t Sector, SecOffset;
   uint16_t entries;
   uint8_t temp8;
   uint8_t * values;

   values=array_in;
   entries=0;
   SecOffset=0; 
   
   do
   {
      if(SecOffset==0) Sector=((Cluster_num-2)*SecPerClus_g)+FirstDataSec_g;
      printf("Cluster#: %9lu\n\rSector#: %9lu\n\rSecOffset: %lu\n\r",Cluster_num,(Sector+SecOffset),SecOffset);
      Read_Sector((Sector+SecOffset), BytesPerSec_g, values);
      print_memory(512,values);
      SecOffset++;
      if(SecOffset==SecPerClus_g)
      {
         Cluster_num=Find_Next_Clus(Cluster_num,values);
         SecOffset=0;
      }
      printf("Press Space to Continue, P to play the song or X to exit\n");
      do
      {
        temp8=getchar();
      }while((temp8!=0x20)&&(temp8!='X')&&(temp8!='P')&&(temp8=='x')&&(temp8=='p'));
	   if((temp8=='P')||(temp8=='p'))
      {
         Play_Song(Cluster_num);
      }
   }while(temp8==0x20);
   putchar(0x0d);
   putchar(0x0a);
return 0;
}




