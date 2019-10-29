#include "STA013.h"
#include "../Drivers/PORT.h"
#include "../Drivers/I2C.h"
#include "../Drivers/Timing.h"
#include "../Drivers/UART.h"
#include <stdio.h>

extern uint8_t code CONFIG;
extern uint8_t code CONFIG2;

uint8_t STA013_Init()
{
	uint8_t error, timeout, reg_addr;
  uint8_t idata send[3], ret_array[3];
  uint8_t * config_data_p;
  uint16_t index;
	uint8_t return_value = STA_NO_ERROR;
	
	uint8_t code STA_cfg_addr[18]={0x54, 0x55,  6, 11,  82,  81, 101, 100, 80, 97, 0x05, 0x0d, 0x18, 0x0c, 0x46, 0x48, 0x7d, 0x72};
  uint8_t code STA_cfg_data[18]={0x07, 0x10,  9,  2, 184,   0,   0,   0,  6,  5, 0xa1, 0x00, 0x04, 0x05, 0x07, 0x07, 0x07, 0x01};

	//Step 3 Debugging Code
	printf("---STA013 Init---\n");

	Port_writePin(YELLOW_LED, LOW);
	error = I2C_Read(0x43, 0x01, 1, 1, ret_array);
	if(ret_array[0] == 0xAC)
	{
		printf("STA Active:%2.2bX\n", ret_array[0]);
	}
	else
	{
		printf("STA Inactive\nError:%2.2bX\nI2C Read:%2.2bX\n", error, ret_array[0]);
		return_value = STA_ERROR_INACTIVE;
	}
	Port_writePin(YELLOW_LED, HIGH);
	
	if(return_value == STA_NO_ERROR)
	{
		 Port_writePin(STA013_RESET, LOW);

		 send[0] = 0x01;   //address of the ID register  
		 timeout = 50;

		 Port_writePin(STA013_RESET, HIGH);    //set STA013 to active
		 do
		 {
				Port_writePin(TRIGGER, LOW); //Trigger for debug
				error = I2C_Read(0x43,1,1,1,ret_array);
				timeout--;
				Port_writePin(TRIGGER, HIGH);
		 }while((error != STA_NO_ERROR)&&(timeout != 0));
		 
		 if(timeout != 0)
		 {
				 printf("Output from ID register: %2.2bX \n\r",ret_array[0]);
		 }



		 if(ret_array[0] == 0xAC)
		 {
			 printf("Sending Configuration Data \n\r"); 
			 index = 0;
			 config_data_p = &CONFIG; //point to STA013 configuration file
			 do
			 {
					reg_addr = config_data_p[index];
					index++;
					send[0] = config_data_p[index];
					index++;
					if(reg_addr != 0xff)
					{
						
						 timeout = 50;
						 do
						 {
								 error = I2C_Write(0x43,reg_addr,1,1,send);
								 timeout--;
								 if(timeout == 0) Port_writePin(RED_LED, LOW);
						 }while((error != STA_NO_ERROR)&&(timeout != 0));
					}
					if((index&0x03E) == 0) UART_Transmit('.');
			 }while((reg_addr != 0xff) && (timeout != 0));
			 printf("First configuration file sent...\n\r");
			 printf("Number of values written: %d \n\r",index>>1);
			 index = 0;
			 config_data_p = &CONFIG2; //point to STA013 configuration file
			 do
			 {
					reg_addr = *(config_data_p+index);
					index++;
					send[0] = *(config_data_p+index);
					index++;
					if(reg_addr != 0xff)
					{
						 timeout = 50;
						 do
						 {
								 error = I2C_Write(0x43,reg_addr,1,1,send);
								 timeout--;
								 if(timeout == 0) Port_writePin(RED_LED, LOW);
						 }while((error != STA_NO_ERROR)&&(timeout != 0));
					}
			 }while((reg_addr != 0xff) && (timeout != 0));
		 error = I2C_Read(0x43,0x71,1,1,ret_array);
			 printf("STA013 SoftVersion: %2.2bX \n\r",ret_array[0]);            
					
			 for(index = 0;index < 18;index++)
			{
				reg_addr = STA_cfg_addr[index];
				send[0] = STA_cfg_data[index];
					timeout = 50;
					do
					{
							error = I2C_Write(0x43,reg_addr,1,1,send);
							timeout--;
							if(timeout == 0) Port_writePin(RED_LED, LOW);
					}while((error != STA_NO_ERROR)&&(timeout != 0));
			printf("Sent to STA013: %2.2bX  %2.2bX \n\r",reg_addr,send[0]);
			reg_addr = STA_cfg_addr[index];
					error = I2C_Read(0x43,reg_addr,1,1,ret_array);
					printf("Output from register: %2.2bX  %2.2bX %2.2bX \n\r",reg_addr,ret_array[0],error);
			}
				printf("Configuration Complete\n\r");
		}

		printf("STA Error:%2.2bX\n",return_value);
		return(return_value);
	}
}