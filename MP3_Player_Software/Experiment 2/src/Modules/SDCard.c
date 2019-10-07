#include "SDCard.h"
#include "../Drivers/Port.h"
#include "../Drivers/SPI.h"


uint8_t SD_Init()
{
	uint8_t error_status = NO_ERRORS;
	uint8_t error_flag = NO_ERRORS;
	uint8_t rec_array[1];
	
	if(error_status == NO_ERRORS)
	{
		SPI_setCSState(LOW);
		error_flag = SD_sendCommand(CMD0, 0x00);
		if(error_flag == NO_ERRORS)
		{
			error_flag = SD_receiveResponse(1, rec_array);
		}
		SPI_setCSState(HIGH);
		if(error_flag != NO_ERRORS)
		{
			error_status = error_flag;
		}
		else if(rec_array[0] != 0x01)
		{
			error_status = RESPONSE_ERROR;
		}	
	}
	
	if(error_status == NO_ERRORS)
	{
		SPI_setCSState(LOW);
		error_flag = SD_sendCommand(CMD8, 0x000001AA);
		if(error_flag == NO_ERRORS)
		{
			error_flag = SD_receiveResponse(1, rec_array);
		}
		SPI_setCSState(HIGH);
		if(error_flag != NO_ERRORS)
		{
			error_status = error_flag;
		}
		else if(rec_array[0] != 0x01)
		{
			error_status = RESPONSE_ERROR;
		}	
	}
	
	//Set nCS
	//Send 74 clock cycles on SCK
	
	//Clear nCS
	//Send CMD0
	//Verify R1 response
	//Set nCS
	//Handle response errors
	
	//Set nCS
	//Clear nCS
	//Send CMD 8
	//Verify R7 response
	//Set nCS
	//Handle response errors
	
	//Clear nCS
	//Send CMD 58
	//Verify R3 response
	//Set nCS
	//Handle response errors
	
	//Clear nCS
	//Send ACMD41
	//Do ACMD41 logic
	
	
	
	return error_status; //Return error status
}

uint8_t SD_readBlock(uint16_t block_number, uint16_t num_bytes, uint8_t * array_out)
{
	uint8_t error_flag = NO_ERRORS;
	uint8_t SPI_value = 0;
	uint8_t index = 0;
	uint8_t error_status = 0;
	
	SPI_setCSState(LOW);
	error_flag = SD_sendCommand(CMD17, block_number);
	
	do
	{
		error_status = SPI_Transfer(0xFF, &SPI_value);
	}while((SPI_value == 0xFF) & (error_status== SPI_NO_ERROR));
	
	if(error_status==SPI_NO_ERROR)
	{
		if(SPI_value == 0x00) //Verify R1 response
		{
			do
			{
				error_status = SPI_Transfer(0xFF, &SPI_value);
			}while((SPI_value == 0xFF) & (error_status==SPI_NO_ERROR));
			
			if(error_status==SPI_NO_ERROR)
			{
				if(SPI_value == 0xFE) //Verify Data Start Token
				{
					for(index = 0; index < num_bytes; index++) //Read data into array
					{
						SPI_Transfer(0xFF, &SPI_value);
						array_out[index] = SPI_value;
					}
					
					SPI_Transfer(0xFF, &SPI_value);
					SPI_Transfer(0xFF, &SPI_value);
					error_status = SPI_Transfer(0xFF, &SPI_value); //Send final 0xFF
				}
				else
				{
					error_status = SD_ERROR_INVALID_DATA_START_TOKEN;
				}
			}
		}
		else
		{
			error_status = SD_ERROR_BAD_RESPONSE;
		}
	}
	
	return error_status; //return error status
}

uint8_t SD_sendCommand(uint8_t CMD_value, uint32_t argument)
{
	uint8_t return_value = NO_ERRORS;
	uint8_t send_value = 0;
	uint8_t error_flag = 0;
	uint8_t rec_value = 0;
	uint8_t index = 0;
	
	if(CMD_value < 64) //verify command is 63 or less (less than 6 bits)
	{
		return_value = NO_ERRORS;
		send_value = 0x40 | CMD_value;
		error_flag = SPI_Transfer(send_value, &rec_value);
		if(error_flag != NO_ERRORS)
		{
			return_value = SPI_ERROR;
		}
		for(index = 0; index < 4; index++)
		{
			if(return_value == NO_ERRORS)
			{
				send_value = (uint8_t) (argument >> (24-(index*8)));
				error_flag = SPI_Transfer(send_value, &rec_value);
				if(error_flag != NO_ERRORS)
				{
					return_value = SPI_ERROR;
				}
			}
		}
		if(CMD_value == CMD0)
		{
			send_value = 0x95;
		}
		else if(CMD_value == CMD8)
		{
			send_value = 0x87;
		}
		else
		{
			send_value = 0x01; //end bit only, CRC7=0
		}
		if(return_value == NO_ERRORS)
		{
			error_flag = SPI_Transfer(send_value, &rec_value);
			if(error_flag != NO_ERRORS)
			{
				return_value = SPI_ERROR;
			}
		}
	}
	else  //if illegal command throw error and exit
	{
		return_value = ILLEGAL_COMMAND;
	}
	
	return return_value; //return eror status	
}

uint8_t SD_receiveResponse(uint8_t num_bytes, uint8_t * rec_array)
{
	//repeatedly send 0xFF and read received value until you have all the data
	
	uint8_t return_value = NO_ERRORS;
	uint8_t timeout = 0;
	uint8_t SPI_value = 0;
	uint8_t error_flag = 0;
	uint8_t index = 0;
	
	do
	{
		error_flag = SPI_Transfer(0xFF, &SPI_value);
		timeout++;
	}while((SPI_value == 0xFF) && (timeout != 0) && (error_flag == NO_ERRORS));
	
	if(error_flag != SPI_NO_ERROR)
	{
		return_value = error_flag;
	}
	else if(timeout == 0)
	{
		return_value = SD_TIMEOUT_ERROR;
	}
	else if((SPI_value & 0xFE) != 0x00) //0x00 and 0x01 are good values
	{
		*rec_array = SPI_value; //return the value to see the error
		return_value = COMM_ERROR;
	}
	else //recieve the rest of the bytes if there are more to receive
	{
		*rec_array = SPI_value; //first received value (R1 response)
		if(num_bytes > 1)
		{
			for(index = 1; index < num_bytes; index++)
			{
				error_flag = SPI_Transfer(0xFF, &SPI_value);
				*(rec_array + index) = SPI_value;
			}
		}
	}
	
	error_flag = SPI_Transfer(0xFF, &SPI_value);
	
	return return_value;
}