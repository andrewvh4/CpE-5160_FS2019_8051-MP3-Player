#include "UtPorts.h"

#include "../Main.h"
#include "../Drivers/Port.h"

// Port Output Functions

void Output_P0(uint8_t port_data) { P0 = port_data; }
void Output_P1(uint8_t port_data) { P1 = port_data; }
void Output_P2(uint8_t port_data) { P2 = port_data; }
void Output_P3(uint8_t port_data) { P3 = port_data; }

// Port Clear Bit Functions

void Clear_bit_P0(uint8_t bit_data) { P0 &= (~bit_data); }
void Clear_bit_P1(uint8_t bit_data) { P1 &= (~bit_data); }
void Clear_bit_P2(uint8_t bit_data) { P2 &= (~bit_data); }
void Clear_bit_P3(uint8_t bit_data) { P3 &= (~bit_data); }

// Port Set Bit Functions

void Set_bit_P0(uint8_t bit_data) { P0 |= bit_data; }
void Set_bit_P1(uint8_t bit_data) { P1 |= bit_data; }
void Set_bit_P2(uint8_t bit_data) { P2 |= bit_data; }
void Set_bit_P3(uint8_t bit_data) { P3 |= bit_data; }