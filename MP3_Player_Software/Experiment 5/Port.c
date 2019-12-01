#include "AT89C51RC2.h"
#include "Port.h"

#include "UART.h"
#include <stdio.h>

uint8_t Port_writePort0(uint8_t bits, uint8_t bitMask);
uint8_t Port_writePort1(uint8_t bits, uint8_t bitMask);
uint8_t Port_writePort2(uint8_t bits, uint8_t bitMask);
uint8_t Port_writePort3(uint8_t bits, uint8_t bitMask);

uint8_t Port_readPort0();
uint8_t Port_readPort1();
uint8_t Port_readPort2();
uint8_t Port_readPort3();

uint8_t Port_writeBus(uint8_t bus, uint8_t bits, uint8_t bitMask)
{
	//send write to specified port
	if(bus == PORT_0) Port_writePort0(bits, bitMask);
	if(bus == PORT_1) Port_writePort1(bits, bitMask);
	if(bus == PORT_2) Port_writePort2(bits, bitMask);
	if(bus == PORT_3) Port_writePort3(bits, bitMask);
	
	return(0);
}

uint8_t Port_writePin(uint8_t pin, uint8_t value)
{
	uint8_t port;
	uint8_t setBit;
	port = pin >> 4; 								//high nibble determines port number
	setBit = (0x01 << (pin&0x0f));	//Bitshift to set pin
	Port_writeBus(port, value == HIGH? setBit : 0x00, ~setBit); //bitmask is ~setBit, buts to set is setBits or 0x00 
	return(0);
}

uint8_t Port_readBus(uint8_t bus)
{
	//Read specified bus
	if(bus == PORT_0) return(Port_readPort0());
	if(bus == PORT_1) return(Port_readPort1());
	if(bus == PORT_2) return(Port_readPort2());
	if(bus == PORT_3) return(Port_readPort3());
	return(0);
}                              
uint8_t Port_readPin(uint8_t pin)
{
	//Read bus (High Nibble) and preform bitmask (Low Nibble)
	return(Port_readBus(pin>>4)&(0x01<<(pin&0x0F)));
}

//writePort for specific ports
uint8_t Port_writePort0(uint8_t bits, uint8_t bitMask)
{
	static uint8_t port0Bits = 0xFF;
	port0Bits = (port0Bits&bitMask)|bits;
	P0 = port0Bits;
	return(0);
}

uint8_t Port_writePort1(uint8_t bits, uint8_t bitMask)
{
	static uint8_t port1Bits = 0xFF;
	port1Bits = (port1Bits&bitMask)|bits;
	P1 = port1Bits;
	return(0);
}

uint8_t Port_writePort2(uint8_t bits, uint8_t bitMask)
{
	static uint8_t port2Bits = 0xFF;
	port2Bits = (port2Bits&bitMask)|bits;
	P2 = port2Bits;
	return(0);
}

uint8_t Port_writePort3(uint8_t bits, uint8_t bitMask)
{
	static uint8_t port3Bits = 0xFF;
	port3Bits = (port3Bits & bitMask) | bits;
	P3 = port3Bits;
	return(0);
}

//readPort for specific ports
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