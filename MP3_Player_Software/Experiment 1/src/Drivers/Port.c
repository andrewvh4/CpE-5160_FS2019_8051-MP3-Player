#include "AT89C51RC2.h"
#include "Port.h"

uint8_t writePort0(uint8_t bits, uint8_t bitMask);
uint8_t writePort1(uint8_t bits, uint8_t bitMask);
uint8_t writePort2(uint8_t bits, uint8_t bitMask);
uint8_t writePort3(uint8_t bits, uint8_t bitMask);

uint8_t readPort0();
uint8_t readPort1();
uint8_t readPort2();
uint8_t readPort3();

uint8_t writePort(uint8_t port, uint8_t bits, uint8_t bitMask)
{
	if(port == 0) writePort0(bits, bitMask);
	if(port == 1) writePort1(bits, bitMask);
	if(port == 2) writePort2(bits, bitMask);
	if(port == 3) writePort3(bits, bitMask);
	
	return(1);
}

uint8_t writeBit(uint8_t Bit, uint8_t value)
{
	uint8_t port = Bit >> 4; //high nibble determines port number
	uint8_t setBit = 0x01 << (Bit&0x0f);
	writePort(port, value == HIGH? setBit : 0x00, ~setBit); //bitmask is ~setBit, buts to set is setBits or 0x00 
	
	return(1);
}

uint8_t readBus(uint8_t bus)
{
	if(bus == 0) return(readPort0());
	if(bus == 1) return(readPort1());
	if(bus == 2) return(readPort2());
	if(bus == 3) return(readPort3());
	return(0);
}                              
uint8_t readBit(uint8_t Bit)
{
	return(readBus(Bit>>4)&(0x01<<(Bit&0x0F)));
}

uint8_t writePort0(uint8_t bits, uint8_t bitMask)
{
	P0 = (P0&bitMask)|bits;
	return(1);
}

uint8_t writePort1(uint8_t bits, uint8_t bitMask)
{
	P1 = (P1&bitMask)|bits;
	return(1);
}

uint8_t writePort2(uint8_t bits, uint8_t bitMask)
{
	P2 = (P2&bitMask)|bits;
	return(1);
}

uint8_t writePort3(uint8_t bits, uint8_t bitMask)
{
	P3 = (P3&bitMask)|bits;
	return(1);
}

uint8_t readPort0()
{
	return(P0);
}