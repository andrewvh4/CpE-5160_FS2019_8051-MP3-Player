#include "PORT.H"
#include "AT89C51RC2.h"

uint8_t writePort0(uint8_t bits, uint8_t bitMask);
uint8_t writePort1(uint8_t bits, uint8_t bitMask);
uint8_t writePort2(uint8_t bits, uint8_t bitMask);
uint8_t writePort3(uint8_t bits, uint8_t bitMask);

uint8_t writePort(uint8_t port, uint8_t bits, uint8_t bitMask)
{
	if(port == 0) writePort0(bits, bitMask);
	if(port == 1) writePort1(bits, bitMask);
	if(port == 2) writePort2(bits, bitMask);
	if(port == 3) writePort3(bits, bitMask);
}

uint8_t writePort0(uint8_t bits, uint8_t bitMask)
{
	P0 = (P0&bitMask)|bits;
}

uint8_t writePort1(uint8_t bits, uint8_t bitMask)
{
	P1 = (P1&bitMask)|bits;
}

uint8_t writePort2(uint8_t bits, uint8_t bitMask)
{
	P2 = (P2&bitMask)|bits;
}

uint8_t writePort3(uint8_t bits, uint8_t bitMask)
{
	P3 = (P3&bitMask)|bits;
}