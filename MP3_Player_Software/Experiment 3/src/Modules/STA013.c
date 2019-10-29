#include "STA013.h"
#include "../Drivers/Port.h"
#include "../Drivers/I2C.h"
#include "../Drivers/Timing.h"
#include "../Drivers/Port.h"
#include <stdio.h>

extern uint8_t code CONFIG;
extern uint8_t code CONFIG2;

uint8_t STA013_Init()
{
	//Step 3 Debugging Code
	uint8_t ret_array[3];
	uint8_t error;
	uint8_t return_value = STA_NO_ERROR;
	
	printf("---STA013 Init\n");

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
	
	if(return_value = STA_NO_ERROR)
	{
		//Write CONFIG and CONFIG1 to STA

	
		//Write second configuration
	
		//Verify CONFIG2 Write by Printing over UART
	}

	printf("STA Error:%2.2bX\n",return_value);
	return(return_value);
}