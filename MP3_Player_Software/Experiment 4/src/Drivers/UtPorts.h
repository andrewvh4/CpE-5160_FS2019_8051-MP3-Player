/*--------------------------------------------------------------------

   UtPorts.h (v1.00)

  ------------------------------------------------------------------
   
   Programmer(s): Andrew Van Horn, Jacob Lipina, David Hinton
   
   Date: 2019-11-03

   Description: Utility Functions for Port Manipulation
--------------------------------------------------------------------*/

#ifndef _UTPORTS_H
#define _UTPORTS_H

#include "Main.h"

// Port Output Functions

void Output_P0(uint8_t port_data);
void Output_P1(uint8_t port_data);
void Output_P2(uint8_t port_data);
void Output_P3(uint8_t port_data);

// Port Clear Bit Functions

void Clear_bit_P0(uint8_t bit_data);
void Clear_bit_P1(uint8_t bit_data);
void Clear_bit_P2(uint8_t bit_data);
void Clear_bit_P3(uint8_t bit_data);

// Port Set Bit Functions

void Set_bit_P0(uint8_t bit_data);
void Set_bit_P1(uint8_t bit_data);
void Set_bit_P2(uint8_t bit_data);
void Set_bit_P3(uint8_t bit_data);

#endif