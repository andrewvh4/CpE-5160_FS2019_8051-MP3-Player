#include "OS.h"
#include "../Main.h"

void sEOS_Init()
{
	uint16_t PRELOAD;
	// Configure Timer2 in auto-reload mode
	T2CON = 0; // Clear Timer2 Control Register 
			   // (enable a-r, timer, disable T2EX, baud rate gen, clear IR flags)

	PRELOAD = (65536 - ((OSC_FREQ * sEOS_INTERVAL_ms) / (OSC_PER_INST * 1000UL)));

	
	RCAP2H = PRELOAD / 256;
	RCAP2L = PRELOAD % 256;

	TF2 = 0; // Clear Overflow Flag.

	// Setup Interrupts
	ET2 = 1; // Enable Timer2 Interrupt
	EA = 1; // Enable Global Interrupt

	// Start Timer2 to cause periodic Interrupts
    TR2 = 1; // Run Timer2

}

uint16_t counter = 0;

void sEOS_ISR() interrupt 5 using 1
{

	TF2 = 0; // Clear Timer Interrupt Flag

	counter ++;
	
	// Perform EOS Tasks
	switch(counter)
	{
		case 1:
			Button_ReadAll();
			break;
		case 2:
			App_UpdateState();
			break;
		case 3:
			App_PerformActions();
			break;
		case 4: //After case 4 reset the shedule counter
			Display_Update();
		default:
			counter = 0;
			break;
	}
}