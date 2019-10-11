/*------------------------------------------------------------------*-

   Port.H (v1.00)

  ------------------------------------------------------------------

   Memory Header - Exp#2
   
   Programmer(s): Anderw Van Horn, Jacob Lipina, David Hinton
   
   Date: 10/1/19

   Description: Manages memory storage on 8051
-------------------------------------------------------------------*/

#ifndef _MEMORY_H
#define _MEMORY_H

#include "Main.h"
#include "Port.h"

void setXRAM(uint16_t XRAMSize);

#define XRAM_SIZE_256  (256)
#define XRAM_SIZE_512  (512)
#define XRAM_SIZE_768  (768)
#define XRAM_SIZE_1024 (1024)

#endif