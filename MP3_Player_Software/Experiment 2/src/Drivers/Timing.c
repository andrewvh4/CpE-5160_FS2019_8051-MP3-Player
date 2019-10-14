#include "Timing.h"

void Timing_delay_1_ms();

void Timing_delay_ms(uint16_t ms)
{
	while(ms > 0)
	{
		Timing_delay_1_ms();
		ms--;
	}
}

void Timing_delay_1_ms()
{
	int y;
  for (y = 0; y <= 240; y++); //120 selected from example "helloWorld" project for delaying 1ms
}