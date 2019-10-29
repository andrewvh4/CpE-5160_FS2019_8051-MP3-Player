#include "SDCard.h"
#include "../Drivers/Port.h"
#include "../Drivers/SPI.h"
#include "../Drivers/Timing.h"
#include "../Main.h"

uint8_t SD_Card_Type;

uint8_t SD_Init()
{
	uint8_t error_status = NO_ERRORS;
	uint8_t error_flag = NO_ERRORS;
	uint8_t index = 0;
	uint16_t timeout = 1;
	uint8_t SPI_Return;
	uint8_t response[8];
	uint32_t argument;
	
	SD_Card_Type=CARD_TYPE_UNKNOWN;
	
	printf("---SD Init\n");
	//set SPI to clock rate of 400KHz or less; This should already be done with SPI_Init
	SPI_setCSState(HIGH); //nCS pin high
	for(index = 0; index < SCK_INIT_BYTES; index++) //Apply at least 74 clock pulses to SCK pin; 
	{
		error_flag = SPI_Transfer(0xFF, &SPI_Return); //80 pulses are applied when 10 bytes are sent through the SPI port.
	}
	
	printf("SD: 80 Pulses\n");
	
	SPI_setCSState(LOW); //nCS pin low
	
	//Send and Check CMD 0
	if(error_status == NO_ERRORS)
	{
		printf("SD: Sending CMD0\n");
		
		error_flag = SD_sendCommand(CMD0, 0x00); //send CMD0 with argument 0x00
		
		
		if(error_flag == NO_ERRORS)
		{
			error_flag = SD_receiveResponse(1, response);
			printf("Response = %2.2bX\n", response[0]);
		}
		if(error_flag != NO_ERRORS)
		{
			error_status = error_flag;
		}
		SPI_setCSState(HIGH);
	}
	
	//Send and Check CMD 8
	if(error_status == NO_ERRORS)
	{
		printf("SD: Sending CMD8\n");
		SPI_setCSState(LOW);
		error_flag = SD_sendCommand(CMD8, 0x000001AA); //send CMD8 with argument 0x000001AA
		if(error_flag == NO_ERRORS)
		{
			error_flag = SD_receiveResponse(5, &response); //Recieve R7 response
			if(error_flag != NO_ERRORS)
			{
				printf("Error Response = %2.2bX\n", response[0]);
				if((error_flag==SD_ERROR_RESPONSE)&&(response[0]==0x05))
				{
					error_status=NO_ERRORS;
					SD_Card_Type=CARD_TYPE_STANDARD_CAPACITY;
					printf("Version 1 SD Card detected.\n");
					printf("Standard Capacity Card detected.\n");
				 }
				 else
				 {
					error_status=error_flag;
				 }
			}
			else
			{
				printf("Response = ");
				for(index=0;index<5;index++)
				{   
					printf("%2.2bX ",response[index]);
				}
				printf("\n");
				if(response[4]!=0xAA)
				{
					error_flag=SD_ERROR_RESPONSE;
					printf("Response Error\n");
				}
				SD_Card_Type=CARD_TYPE_VERSION_2;
				printf("Version 2 SD Card detected.\n");
			}
		}	
		SPI_setCSState(HIGH);		
	}
	
	//Send and Check CMD58
	if(error_status == NO_ERRORS)
	{
		printf("SD: Sending CMD 58\n");
		SPI_setCSState(LOW);
		error_flag = SD_sendCommand(CMD58, 0x00); //send CMD58 with argument 0x00
		
		
		if(error_flag == NO_ERRORS)
		{
			error_flag = SD_receiveResponse(5, &response); //Recieve R3 response
			printf("Response = ");
			for(index=0;index<5;index++)
			{   
				 printf("%2.2bX ",response[index]);
			}
			printf("\n");
			if((response[2]&0xFC)!=0xFC)
			{
			   error_flag=SD_ERROR_VOLTAGE;
			   printf("Voltage Error\n");
			}
		}
		if(error_flag != NO_ERRORS)
		{
			error_status = error_flag;
		}
		SPI_setCSState(HIGH);
	}
	
	if(error_status==NO_ERRORS)
    {
		if(SD_Card_Type==CARD_TYPE_VERSION_2)
		{
			argument=0x40000000;
		}
		else
		{
			argument=0;
		}
	 
		timeout = 0;
		printf("SD: Sending ACMD41\n");
		do
		{
			SPI_setCSState(LOW);
			error_flag = SD_sendCommand(CMD55, 0x00); //send ACMD41 which must be preceeded by CMD55 with arg 0x00
			if(error_flag == NO_ERRORS)
			{
				error_flag = SD_receiveResponse(1, &response); //Recieve R1 response
			}
			if((response[0]==0x01)||(response[0]==0x00))
			{
				error_flag=SD_sendCommand(ACMD41,argument);
			}
			if(error_flag==NO_ERRORS) 
			{
				SD_receiveResponse(1,response);
			}
			SPI_setCSState(HIGH); 
			timeout++;
			if(timeout==0) error_flag=SD_ERROR_TIMEOUT;
		}while(((response[0]&0x01)==0x01)&&(error_flag==NO_ERRORS));
		
		printf("Response = %2.2bX\n",response[0]);
		printf("Timeout = %2.2bX\n",timeout);
		
		if(error_flag!=SPI_NO_ERROR)
		{		
			error_status=error_flag;
		}
		
	}
 
	printf("SD Error:%2.2bX\n",error_status);
	return error_status; //Return error status
}

uint8_t SD_readBlock(uint32_t block_number, uint16_t num_bytes, uint8_t * array_out)
{
	uint8_t error_flag = NO_ERRORS;
	uint8_t SPI_value = 0;
	uint8_t index = 0;
	uint8_t error_status = 0;
	
	printf("Reading Block %2.2bX\n", block_number);
	printf("Sending CMD 7\n");
	SPI_setCSState(LOW);
	error_flag = SD_sendCommand(CMD17, (block_number<<SD_Card_Type));
	
	do
   {
      error_flag=SPI_Transfer(0xFF,&SPI_value);
      index++;
   }while(((SPI_value&0x80)==0x80)&&(index!=0)&&(error_flag==NO_ERRORS));
   if(error_flag!=NO_ERRORS)
   {
      error_status=SPI_ERROR;
   }
   else if(index==0)
   {
      error_status=SPI_ERROR_TIMEOUT;
   }
   else
   {
         if (SPI_value==0x00)
		{
			index=0;
			do
			{ 
				error_flag=SPI_Transfer(0xFF,&SPI_value); 
				index++;
			}while((SPI_value==0xFF)&&(index!=0)&&(error_flag==NO_ERRORS));          // wait for data token response  
			if(error_flag!=NO_ERRORS)
			{
				error_status=SPI_ERROR;
			}
			else if(index==0)
			{
				error_status=SD_ERROR_TIMEOUT;
			}
			else if(SPI_value==0xfe)
			{
				for(index=0;index<num_bytes;index++)
				{
				error_flag=SPI_Transfer(0xFF,&SPI_value);
				*(array_out + index)=SPI_value;
				}
				error_flag=SPI_Transfer(0xFF,&SPI_value); // discard byte 1 of CRC16	
				error_flag=SPI_Transfer(0xFF,&SPI_value); // discard byte 2 of CRC16
			}
			else
			{
				error_status=SD_ERROR_DATA;
			}
		}
		else
		{
			error_status=SD_ERROR_RESPONSE;
		}
    }
	
	SPI_setCSState(HIGH);
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
		send_value = 0x40 | CMD_value;
		error_flag = SPI_Transfer(send_value, &rec_value);
		if((error_flag)==NO_ERRORS)
	    {
	        send_value=argument>>24;   // MSB
	        error_flag=SPI_Transfer(send_value,&rec_value);
	    }
	    else
	    {
		    return_value=SPI_ERROR;
		}
		if((return_value==NO_ERRORS)&&(error_flag==NO_ERRORS))
		{
		    argument=argument & 0x00ffffff;
		    send_value=argument>>16;  // BYTE2
		    error_flag=SPI_Transfer(send_value,&rec_value);
		}
		else
		{
		    return_value=SPI_ERROR;
		}
		if((return_value==NO_ERRORS)&&(error_flag==NO_ERRORS))
		{
		    argument=argument & 0x0000ffff;
		    send_value=argument>>8;   // BYTE1
		    error_flag=SPI_Transfer(send_value,&rec_value);
		}
		else
		{
		    return_value=SPI_ERROR;
		}     
		if((return_value==NO_ERRORS)&&(error_flag==NO_ERRORS))
		{
		    send_value=argument & 0x000000ff;  // LSB
		    error_flag=SPI_Transfer(send_value,&rec_value);
		}
		else
		{
		   return_value=SPI_ERROR;
		}
		if((return_value==NO_ERRORS)&&(error_flag==NO_ERRORS))
		{         
		     if (CMD_value == 0)
		     {
		         send_value=0x95;  // CRC7 and end bit for CMD0
		     }
		     else if (CMD_value == 8)
		     {
		         send_value=0x87;   // CRC7 and end bit for CMD8
		     }
		     else
		     {
		         send_value=0x01;  // end bit only for other commands
		     }
		     error_flag=SPI_Transfer(send_value,&rec_value);
		}
		else
	    {
	        return_value=SD_ERROR_ILLEGAL_COMMAND;
	    }
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
	}while(((SPI_value&0x80)==0x80)&&(timeout!=0)&&(error_flag==SPI_NO_ERROR));
	
	if(error_flag != SPI_NO_ERROR)
	{
		return_value = SPI_ERROR;
	}
	else if(timeout == 0)
	{
		return_value = SD_ERROR_TIMEOUT;
	}
	else if((SPI_value & 0xFE) != 0x00) //0x00 and 0x01 are good values
	{
		*rec_array = SPI_value; //return the value to see the error
		return_value = SD_ERROR_COMM;
	}
	else //recieve the rest of the bytes if there are more to receive
	{
		*rec_array = SPI_value; //first received value (R1 response)
		if((SPI_value==0x00)||(SPI_value==0x01))
     	{
			if(num_bytes > 1)
			{
				for(index = 1; index < num_bytes; index++)
				{
					error_flag = SPI_Transfer(0xFF, &SPI_value);
					*(rec_array + index) = SPI_value;
				}
			}
		}
	}
	
	error_flag = SPI_Transfer(0xFF, &SPI_value);
	
	return return_value;
}