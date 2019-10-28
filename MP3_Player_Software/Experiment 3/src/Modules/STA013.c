#include "STA013.h"
#include "../Drivers/Port.h"
#include "../Drivers/I2C.h"
#include "../Drivers/Timing.h"
#include <stdio.h>

uint8_t STA013_Init()
{
	//Step 3 Debugging Code
	uint8_t ret_array[3];
	uint8_t error;
	
	while(1)
	{
		error = I2C_Read(0x43, 0x00, 3, 3, ret_array);
		printf("I2C Read:%2.2bX:%2.2bX;%2.2bX;%2.2bX;", error, ret_array[0], ret_array[1], ret_array[2]);
		Timing_delay_ms(100);
	}
	
}