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
	
	while(1)
	{
		Port_writePin(YELLOW_LED, HIGH);
		error = I2C_Read(0x43, 0x00, 3, 3, ret_array);
		printf("I2C Read:%2.2bX:%2.2bX;%2.2bX;%2.2bX\n", error, ret_array[0], ret_array[1], ret_array[2]);
		Port_writePin(YELLOW_LED, LOW);
		Timing_delay_ms(100);
	}
	printf("STA Init\n");
	
	//Write CONFIG and CONFIG1 to STA
	do
	{
		
	} while(CONFIG[i] != 0xff);
	CONFIG[0];
	
	//Write second configuration
	
	//Verify CONFIG2 Write by Printing over UART
}