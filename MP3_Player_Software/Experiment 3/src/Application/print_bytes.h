#ifndef _print_bytes_H
#define _print_bytes_H

#include "../main.h"
#include "../drivers/UART.h"



// ------ Public function prototypes -------------------------------

void print_hex(uint8_t in);

void print_16bytes(uint8_t * array_in);

void print_memory(uint8_t * array_in, uint16_t number_of_bytes);

#define space (0x20)
#define CR (0x0D)
#define LF (0x0A)
#define BS (0x08)
#define DEL (0x7F)


#endif