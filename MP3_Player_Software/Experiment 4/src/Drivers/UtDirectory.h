/*--------------------------------------------------------------------

   UtDirectory.h (v1.01)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-10-01

   Description: Utility functions to interact with file directories.
--------------------------------------------------------------------*/

#ifndef _UTDIRECTORY_H
#define _UTDIRECTORY_H

#include "Main.h"

//------- Public Constants  -----------------------------------------
#define FAT32 4
#define FAT16 4

// ------ Public function prototypes -------------------------------

uint16_t Print_Directory(uint32_t sector_num, uint8_t* array_in);

uint32_t Read_Directory_Entry(uint32_t sector_num, uint16_t Entry, uint8_t* array_in);

#endif