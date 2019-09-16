#include "Timing.h"

void delay_1_ms();

void delay_ms(uint16_t ms)
{
	while(ms > 0)
	{
		delay_1_ms();
		ms--;
	}
}

void delay_1_ms()
{
	//Create while loop to delay for 1 ms
}