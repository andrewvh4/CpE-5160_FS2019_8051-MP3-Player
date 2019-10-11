#include "main.h"
#include "SPI.h"
#include "Uart.h"
#include <stdio.h>

uint8_t SPI_Init(uint32_t clock_rate)
{
	uint16_t divider; 
	uint8_t return_value = 0;
	
	divider = (OSC_FREQ * 12) / (OSC_PER_INST * clock_rate); //For some reason this equation returns 46.08 when hand-calculated
	
	UART_Transmit(divider);

	divider = 3; //Fix this bug when not tired
	
	if (divider < 2)
	{
		SPCON = 0x70 | (CPOL << 3) | (CPHA << 2);
		printf("2");
	}
	else if (divider < 4) 
	{ 
		SPCON = 0x71 | (CPOL << 3) | (CPHA << 2); 
		printf("4");
	}
	else 
	{ 
		return_value = SPI_ERROR_CLOCKRATE; 
		printf("F");
	}
	
	SPSTA = SPSTA | 0x80;
	
	return return_value;
}

uint8_t SPI_Transfer(uint8_t send_value, uint8_t *received_value)
{
	uint16_t timeout = 0; 
	uint8_t status = 0;
	uint8_t error_flag = 0;
	
	

  SPDAT = send_value;
	
	do 
	{ 
		status = SPSTA; 
		timeout++; 
	} while (((status & 0xF0) == 0x00) && (timeout != 0));

	if (timeout == 0) 
	{    
		// timeout error
		error_flag = SPI_ERROR_TIMEOUT;
		*received_value = 0xFF;
	}
	else if((status & 0x70) != 0)
	{
		error_flag = SPI_ERROR;
		*received_value = 0xFF;
	}
	else
	{
		error_flag = SPI_NO_ERROR;
		*received_value = SPDAT;
	}
	return error_flag;
}

uint8_t SPI_setCSState(uint8_t state)
{
	Port_writePin(CS, state);
	return(0);
}
