/*--------------------------------------------------------------------

   OS.h (v1.01)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-12-01

   Description: Defines functions SEOS operation
--------------------------------------------------------------------*/
#ifndef _OS
#define _OS

#include "../Main.h"
#include "Buttons.h"

void sEOS_Init(); //Initializes sEOS in Timer 2
void sEOS_ISR();  //ISR for timer interrupt 2

#define sEOS_INTERVAL_ms 10
#define sEOS_NUMBER_OF_ACTIONS 4
#define sEOS_LOOP_INTERVAL_ms (sEOS_INTERVAL_ms*sEOS_NUMBER_OF_ACTIONS)

#endif