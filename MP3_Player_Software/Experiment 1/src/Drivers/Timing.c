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
	int y;
  for (y = 0; y <= 120; y++); //120 selected from example "helloWorld" project for delaying 1ms
}