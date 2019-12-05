/*------------------------------------------------------------------*-

   STA013.h (v1.00)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
	 
	 Hardware: STA013
   
   Date: 2019-10-22

   Description: Defines functions for STA013 operation
-*------------------------------------------------------------------*/
// ------ Public function prototypes -------------------------------
#ifndef _STA013_H
#define _STA013_H

#include "../Main.h"

uint8_t STA013_Init();

#define STA_NO_ERROR 0x00
#define STA_ERROR_INACTIVE 0x01

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/



