#include "main.h"
#include "SPI.h"
#include "Uart.h"
#include <stdio.h>

uint8_t SPI_Init(uint32_t clock_freq)
{
	uint16_t divider; 
	uint8_t return_value = 0;
	
	divider = (OSC_FREQ * 6) / (OSC_PER_INST * clock_freq); //For some reason this equation returns 46.08 when hand-calculated
	
	UART_Transmit(divider);

	divider = 3; //Fix this bug when not tired
	
	if(divider<=2)
  {
     SPCON=0x70;
  }
  else if((divider>2)&&(divider<=4))
  {
     SPCON=0x71;
  }
  else if((divider>4)&&(divider<=8))
  {
     SPCON=0x72;
  }
  else if((divider>8)&&(divider<=16))
  {
     SPCON=0x73;
  } 
  else if((divider>16)&&(divider<=32))
  {
     SPCON=0xF0;
  }
  else if((divider>32)&&(divider<=64))
  {
     SPCON=0xF1;
  }
  else if((divider>64)&&(divider<=128))
  {
     SPCON=0xF2;
  }
  else  // if the SPI clock rate is too slow, a divider cannot be found
  {
    return_value = SPI_ERROR_CLOCKRATE; 
  }
	
	SPCON = SPCON | (CPOL << 3) | (CPHA << 2);
	
	return return_value;
}

uint8_t SPI_Transfer(uint8_t send_value, uint8_t *received_value)
{
	uint8_t timeout = 0; 
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
	CS = state;
	return(0);
}