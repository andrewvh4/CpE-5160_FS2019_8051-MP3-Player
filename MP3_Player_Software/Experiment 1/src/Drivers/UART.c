#include "../main.h"
#include "UART.h"

//Calculate baud rate generator values for BaudRate 9600

uint8_t UART_Init_9600()
{
	//Initialize hardware registers (PCON, SCON, BDRCON)
	PCON &= 0x3F;
	PCON |= (SMOD1 << 7);

	//SCON = set to UART mode 1 and enable receive (Clear RI and TI)
	//SCON = [(SM0)(SM1)(SM2)(REW)(TB8)(RB8)(TI)(RI)]
	SCON = 0x50; //0b01010000
	
	//Disable UART interrupts
	ES = 0;
	
	//Set TI bit 
	TI = 1;

	//Set up baud rate generator
	//1) Disable baud rate generator
	BDRCON = 0;

	//2) Set BRL value, reload value
	BRL = (uint8_t)(256-(((1+(5*SPD))*(1+SMOD1)*OSC_FREQ)/(32*OSC_PER_INST*(uint32_t)BAUD_RATE)));

	//3)Set BDRCON register
	BDRCON = 0x1C | (SPD << 1);
	
	return(0);
}

uint8_t UART_Transmit(uint8_t character)
{
	//Wait for TI to be set
	while(TI==0);

	//Write value to be sent to SBUF
	SBUF = character;

	//Clear the TI bit to indicate that SBUF is full
	TI = 0;

	return(character);
}

uint8_t UART_Receive()
{
	uint8_t temp8;
	//Wait for RI to be ste
	while(RI==0);

	//Read the received value from SBUF
	temp8 = SBUF;

	//Clear the RI flag to indicate the value was read from SBUF
	RI = 0;

	return(temp8);
}