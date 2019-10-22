#include "../main.h"
#include "I2C.h"
#Include <stdio.h>


uint8_t I2C_Read(uint8_t address, uint32_t int_address, uint8_t int_address_size, uint8_t num_bytes,uint8_t * ret_array);
{
	//Write start address
	
	//Create start condition
	
	//Send device address with 1 in lsb
	
	//receive ACK from slave
	
	//receive data byte from slave
	
	//keep senging ACK until all bytes are read, then send NACK
	
}

uint8_t I2C_Write(uint8_t address, uint32_t int_address, uint8_t int_address_size, uint8_t num_bytes,uint8_t * write_array)
{
	//Create a start condition
	
	//Send device address with 0 in lsb
	
	//receive ACK from slave
	
	//Send register address
	
	//receive ACK from slave
	
	//Send data to register
	
	//Receive ACK
	
	//Create stop condition
}