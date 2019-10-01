#include "SDCard.h"

uint8_t SD_sendCommand(uint8_t command, uint32_t argument);
uint8_t SD_receiveResponse(uint8_t number_of_bytes, uint8_t * array_name);

uint8_t SD_Init()
{
	//Set nCS
	//Send 74 clock cycles on SCK
	//Clear nCS
	//Send CMD0
	//Verify R1 response
	//Handle response errors
	
	//Set nCS
	//Clear nCS
	//Send CMD 8
	//Verify R7 response
	//Set nCS
	//Handle response errors
	
	//Clear nCS
	//Send CMD 8
	//Verify R3 response
	//Set nCS
	//Handle response errors
	
	//Clear nCS
	//Send ACMD41
	//Do ACMD41 logic
	
	//Return error status
	
	return(0);
}

uint8_t SD_readBlock(uint16_t number_of_bytes, uint8_t * array)
{
	//Verify R1 response
	//Verify Data Start Token
	//Read data into array
	//Send final 0xFF
	//return error status
	return(0);
}

uint8_t SD_sendCommand(uint8_t command, uint32_t argument)
{
	//verify command is 63 or less (less than 6 bits), if illegal throw error and exit
	//Append start and transmission bits
	//send first byte
	//Send argument
	//send checksum
	//return eror status
	return(0);	
}

uint8_t SD_receiveResponse(uint8_t number_of_bytes, uint8_t * array_name)
{
	//repeatedly send 0xFF and read received value until you have all the data
	return(0);
}