#include "AT89C51RC2.h"
#include "Port.h"

uint8_t Port_writePort0(uint8_t bits, uint8_t bitMask);
uint8_t Port_writePort1(uint8_t bits, uint8_t bitMask);
uint8_t Port_writePort2(uint8_t bits, uint8_t bitMask);
uint8_t Port_writePort3(uint8_t bits, uint8_t bitMask);

uint8_t Port_readPort0();
uint8_t Port_readPort1();
uint8_t Port_readPort2();
uint8_t Port_readPort3();

uint8_t Port_writePort(uint8_t port, uint8_t bits, uint8_t bitMask)
{
	if(port == 0) Port_writePort0(bits, bitMask);
	if(port == 1) Port_writePort1(bits, bitMask);
	if(port == 2) Port_writePort2(bits, bitMask);
	if(port == 3) Port_writePort3(bits, bitMask);
	
	return(1);
}

uint8_t Port_writePin(uint8_t pin, uint8_t value)
{
	uint8_t port = pin >> 4; //high nibble determines port number
	uint8_t setBit = 0x01 << (pin&0x0f);
	Port_writePort(port, value == HIGH? setBit : 0x00, ~setBit); //bitmask is ~setBit, buts to set is setBits or 0x00 
	
	return(1);
}

uint8_t Port_readBus(uint8_t bus)
{
	if(bus == 0) return(Port_readPort0());
	if(bus == 1) return(Port_readPort1());
	if(bus == 2) return(Port_readPort2());
	if(bus == 3) return(Port_readPort3());
	return(0);
}                              
uint8_t Port_readPin(uint8_t pin)
{
	return(Port_readBus(pin>>4)&(0x01<<(pin&0x0F)));
}

uint8_t Port_writePort0(uint8_t bits, uint8_t bitMask)
{
	P0 = (P0&bitMask)|bits;
	return(1);
}

uint8_t Port_writePort1(uint8_t bits, uint8_t bitMask)
{
	P1 = (P1&bitMask)|bits;
	return(1);
}

uint8_t Port_writePort2(uint8_t bits, uint8_t bitMask)
{
	P2 = (P2&bitMask)|bits;
	return(1);
}

uint8_t Port_writePort3(uint8_t bits, uint8_t bitMask)
{
	P3 = (P3&bitMask)|bits;
	return(1);
}

uint8_t Port_readPort0()
{
	return(P0);
}

uint8_t Port_readPort1()
{
	return(P1);
}

uint8_t Port_readPort2()
{
	return(P2);
}

uint8_t Port_readPort3()
{
	return(P3);
}