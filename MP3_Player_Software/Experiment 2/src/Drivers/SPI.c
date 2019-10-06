#include "SPI.h"

uint8_t SPI_Init(uint32_t clock_rate)
{
	uint16_t divider = (OSC_FREQ * 6) / (OSC_PER_INST * clock_rate);
	uint8_t clock_rate_error = 1;
	uint8_t return_value = 0;

	if (divider < 2)
	{
		SPCON = 0x70 | (CPOL << 3) | (CPHA << 2);
	}
	else if (divider < 4) 
	{ 
		SPCON = 0x71 | (CPOL << 3) | (CPHA << 2); 
	}
	else 
	{ 
		return_value = clock_rate_error; 
	}
	return return_value;
}

uint8_t SPI_Transfer(uint8_t send_value, uint8_t *received_value)
{
	SPDAT = send_value;
	uint16_t timeout = 0; 
	uint8_t status = 0;
	uint8_t error_flag = 0;
	uint8_t no_errors = 0;
	uint8_t timeout_error = 1;
	uint8_t SPI_error = 2;

	
	do 
	{ 
		status = SPSTA; 
		timeout++; 
	} while (((status & 0xF0) == 0) && (timeout != 0));

	if (timeout == 0) 
	{    
		// timeout error
		error_flag = timeout_error;
		*received_value = 0xFF;
	}
	else if((status & 0x70) != 0)
	{
		error_flag = SPI_error;
		*received_value = 0xFF;
	}
	else
	{
		error_flag = no_errors;
		*received_value = SPDAT;
	}
	return error_flag;
}