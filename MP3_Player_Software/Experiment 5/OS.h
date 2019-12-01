/*--------------------------------------------------------------------

   OS.h (v1.01)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-12-01

   Description: Defines functions SEOS operation
--------------------------------------------------------------------*/
#ifndef _OS
#define _OS

#include "Main.h"

void sEOS_Init();
void sEOS_ISR();

#define sEOS_Interval_ms 10

#endif