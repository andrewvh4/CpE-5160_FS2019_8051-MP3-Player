#include "SDCard.h"
#include "Port.h"
#include "SPI.h"

#include "AT89C51RC2.h"
#include "PORT.H"
#include "main.h"
#include "SDCard.h"
#include "print_bytes.h"
#include "SPI.h"
#include <stdio.h>

static uint8_t  idata SD_Card_Type;

uint8_t SD_sendCommand(uint8_t command, uint32_t argument)
{
 uint8_t send_val, return_val, SPI_return, spi_error_flag, counter;

 return_val=SD_NO_ERROR;
 if(command<64)
 {
   send_val=command | 0x40;
   spi_error_flag=SPI_Transfer(send_val,&SPI_return);

	 if(spi_error_flag!=SPI_NO_ERROR)
	 {
		 return_val = spi_error_flag;
	 }
		 
	 counter = 3;
	 while((return_val==SD_NO_ERROR)&& counter >= 0)
	 {
		 send_val=argument >> (counter*8);  
		 argument = argument | 0xff << (counter*8);
     spi_error_flag=SPI_Transfer(send_val,&SPI_return);
		 
		 if(spi_error_flag!=SPI_NO_ERROR)
		 {
			 return_val = spi_error_flag;
		 }
		 counter --;
	 }
	 
	 
   if((return_val==SD_NO_ERROR))
   {         
      if (command == 0)
      {
         send_val=0x95;  // CRC7 and end bit for CMD0
      }
      else if (command == 8)
      {
         send_val=0x87;   // CRC7 and end bit for CMD8
      }
      else
      {
         send_val=0x01;  // end bit only for other commands
      }
      spi_error_flag=SPI_Transfer(send_val,&SPI_return);
    }
 }
 else
 {
   return_val=SD_ERROR_ILLEGAL_COMMAND;
 }
 return return_val;  
}


uint8_t SD_receiveResponse(uint8_t num_bytes, uint8_t * response_out)
{
   uint8_t index,return_val,spi_error_flag,SPI_return;

   return_val=SPI_NO_ERROR;
   do
   {
      spi_error_flag=SPI_Transfer(0xFF,&SPI_return);
      index++;
   }while(((SPI_return&0x80)==0x80)&&(index!=0)&&(spi_error_flag==SPI_NO_ERROR));
   
	 if(spi_error_flag!=SPI_NO_ERROR)
   {
      return_val=spi_error_flag;
   }
   else if(index==0)
   {
      return_val=SPI_ERROR_TIMEOUT;
   }
   else
   {
     *response_out=SPI_return;
     if((SPI_return==0x00)||(SPI_return==0x01))
     {
       if(num_bytes>1)
       {
         for(index=1;index<num_bytes;index++)
         {
            spi_error_flag=SPI_Transfer(0xFF,&SPI_return);
            *(response_out+index)=SPI_return;
         }
       }
     }
     else
     {
        return_val=SD_ERROR_RESPONSE;
     }
   }
   spi_error_flag=SPI_Transfer(0xFF,&SPI_return);  // send 8 more clock cycles to complete read
   return return_val;
}

uint8_t SD_readBlock(uint16_t num_bytes, uint8_t * array_out)
{
   uint8_t spi_error_flag,return_val,SPI_return;
   uint16_t index;
   index=0;
   return_val=SD_NO_ERROR;
   do
   {
      spi_error_flag=SPI_Transfer(0xFF,&SPI_return);
      index++;
   }while(((SPI_return&0x80)==0x80)&&(index!=0)&&(spi_error_flag==SPI_NO_ERROR));
   if(spi_error_flag!=SPI_NO_ERROR)
   {
      return_val=SPI_ERROR;
   }
   else if(index==0)
   {
      return_val=SPI_ERROR_TIMEOUT;
   }
   else
   {
     if (SPI_return==0x00)
     {
        index=0;
				do
        { 
           spi_error_flag=SPI_Transfer(0xFF,&SPI_return); 
           index++;
        }while((SPI_return==0xFF)&&(index!=0)&&(spi_error_flag==SPI_NO_ERROR));          // wait for data token response  
        if(spi_error_flag!=SPI_NO_ERROR)
        {
           return_val=SPI_ERROR;
        }
        else if(index==0)
        {
          return_val=SPI_ERROR_TIMEOUT;
        }
        else if(SPI_return==0xfe)
        {
          for(index=0;index<num_bytes;index++)
          {
             spi_error_flag=SPI_Transfer(0xFF,&SPI_return);
             *(array_out + index)=SPI_return;
          }
          spi_error_flag=SPI_Transfer(0xFF,&SPI_return); // discard byte 1 of CRC16	
          spi_error_flag=SPI_Transfer(0xFF,&SPI_return); // discard byte 2 of CRC16
        }
	    else
	    {
	      return_val=SD_ERROR_INVALID_DATA_START_TOKEN;
	    }
     }
     else
     {
        return_val=SD_ERROR_RESPONSE;
     }
   }
   spi_error_flag=SPI_Transfer(0xFF,&SPI_return);// send 8 more clock cycles to complete read
   return return_val;
}

uint8_t SD_Init()
{
	uint8_t error_status = SD_NO_ERROR;
	uint8_t spi_error_flag = SD_NO_ERROR;
	uint8_t rec_array[1];
	
	if(error_status == SD_NO_ERROR)
	{
		SPI_setCSState(0);
		spi_error_flag = SD_sendCommand(CMD0, 0x00);
		if(spi_error_flag == SD_NO_ERROR)
		{
			spi_error_flag = SD_receiveResponse(1, rec_array);
		}
		SPI_setCSState(1);
		if(spi_error_flag != SD_NO_ERROR)
		{
			error_status = spi_error_flag;
		}
		else if(rec_array[0] != 0x01)
		{
			error_status = SD_ERROR_RESPONSE;
		}	
	}
	
	if(error_status == SD_NO_ERROR)
	{
		SPI_setCSState(0);
		spi_error_flag = SD_sendCommand(CMD8, 0x000001AA);
		if(spi_error_flag == SD_NO_ERROR)
		{
			spi_error_flag = SD_receiveResponse(1, rec_array);
		}
		SPI_setCSState(1);
		if(spi_error_flag != SD_NO_ERROR)
		{
			error_status = spi_error_flag;
		}
		else if(rec_array[0] != 0x01)
		{
			error_status = SD_ERROR_RESPONSE;
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

#if 0
uint8_t SD_Init()
{
	uint8_t error_status = SD_NO_ERROR;
	uint8_t spi_error_flag = SD_NO_ERROR;
	uint8_t rec_array[1];
	
	if(error_status == SD_NO_ERROR)
	{
		SPI_setCSState(0);
		spi_error_flag = SD_sendCommand(CMD0, 0x00);
		if(spi_error_flag == SD_NO_ERROR)
		{
			spi_error_flag = SD_receiveResponse(1, rec_array);
		}
		SPI_setCSState(1);
		if(spi_error_flag != SD_NO_ERROR)
		{
			error_status = spi_error_flag;
		}
		else if(rec_array[0] != 0x01)
		{
			error_status = RESPONSE_ERROR;
		}	
	}
	
	if(error_status == SD_NO_ERROR)
	{
		SPI_setCSState(0);
		spi_error_flag = SD_sendCommand(CMD8, 0x000001AA);
		if(spi_error_flag == SD_NO_ERROR)
		{
			spi_error_flag = SD_receiveResponse(1, rec_array);
		}
		SPI_setCSState(1);
		if(spi_error_flag != SD_NO_ERROR)
		{
			error_status = spi_error_flag;
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
	uint8_t spi_error_flag = SD_NO_ERROR;
	uint8_t SPI_value = 0;
	uint8_t index = 0;
	uint8_t error_status = 0;
	
	SPI_setCSState(0);
	spi_error_flag = SD_sendCommand(CMD17, block_number);
	
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

uint8_t SD_sendCommand(uint8_t CMD_value, uint32_t argumentent)
{
	uint8_t return_value = SD_NO_ERROR;
	uint8_t send_value = 0;
	uint8_t spi_error_flag = 0;
	uint8_t rec_value = 0;
	uint8_t index = 0;
	
	if(command<64)
	 {
		 send_value=CMD_value | 0x40;
		 spi_error_flag=SPI_Transfer(send_value,&rec_value);
		 if((spi_error_flag)==SPI_NO_ERROR)
		 {
			 send_value=argumentent>>24;   // MSB
			 spi_error_flag=SPI_Transfer(send_value,&rec_value);
		 }
		 else
		 {
			 return_value=SPI_error;
		 }
		 if((return_value==SD_NO_ERROR)&&(spi_error_flag==SPI_NO_ERROR))
		 {
			 argument=argument & 0x00ffffff;
			 send_value=argumentent>>16;  // BYTE2
			 spi_error_flag=SPI_Transfer(send_value,&rec_value);
		 }
		 else
		 {
			 return_value=SPI_error;
		 }
		 if((return_value==SD_NO_ERROR)&&(spi_error_flag==SPI_NO_ERROR))
		 {
			 argument=argument & 0x0000ffff;
			 send_value=argumentent>>8;   // BYTE1
			 spi_error_flag=SPI_Transfer(send_value,&rec_value);
		 }
		 else
		 {
			 return_value=SPI_error;
		 }     
		 if((return_value==SD_NO_ERROR)&&(spi_error_flag==SPI_NO_ERROR))
		 {
			 send_value=argumentent & 0x000000ff;  // LSB
			 spi_error_flag=SPI_Transfer(send_value,&rec_value);
		 }
		 else
		 {
			 return_value=SPI_error;
		 }
		 if((return_value==SD_NO_ERROR)&&(spi_error_flag==SD_NO_ERROR))
		 {         
				if (command == 0)
				{
					 send_value=0x95;  // CRC7 and end bit for CMD0
				}
				else if (command == 8)
				{
					 send_value=0x87;   // CRC7 and end bit for CMD8
				}
				else
				{
					 send_value=0x01;  // end bit only for other commands
				}
				spi_error_flag=SPI_Transfer(send_value,&rec_value);
			}
	 }
	 else
	 {
		 return_value=illegal_commandargument;
	 }	
	return return_value; //return eror status	
}

uint8_t SD_receiveResponse(uint8_t num_bytes, uint8_t * rec_array)
{
	//repeatedly send 0xFF and read received value until you have all the data
	
	uint8_t return_value = SD_NO_ERROR;
	uint8_t timeout = 0;
	uint8_t SPI_value = 0;
	uint8_t spi_error_flag = 0;
	uint8_t index = 0;
	
	do
	{
		spi_error_flag = SPI_Transfer(0xFF, &SPI_value);
		timeout++;
	}while((SPI_value == 0xFF) && (timeout != 0) && (spi_error_flag == SD_NO_ERROR));
	
	if(spi_error_flag != SPI_NO_ERROR)
	{
		return_value = spi_error_flag;
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
				spi_error_flag = SPI_Transfer(0xFF, &SPI_value);
				*(rec_array + index) = SPI_value;
			}
		}
	}
	
	spi_error_flag = SPI_Transfer(0xFF, &SPI_value);
	
	return return_value;
}

#endif