#include "Memory.h"

void setXRAM(uint16_t XRAMSize)
{
	AUXR = AUXR & 0xFD; //Set bit EXTRAM low
	
	if(XRAMSize = XRAM_SIZE_1024)
	{
		AUXR = AUXR | 0x0C; //set XRS1 High, XRS0 LOW
	}
	return;
	//Set AUXR to set XRAM to specified size
}