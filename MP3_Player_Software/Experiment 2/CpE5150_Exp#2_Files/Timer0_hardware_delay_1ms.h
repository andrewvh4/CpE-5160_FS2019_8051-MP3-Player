#ifndef _Timer0_Hardware_Delay_1ms_H
#define _Timer0_Hardware_Delay_1ms_H

#include "Main.H"

/***** Public Constants ******/
#define one_msec (1)

#define Timer_Reload_1ms (65536-((OSC_FREQ*one_msec)/(1020*OSC_PER_INST)))  
#define Timer_Reload_1ms_THx (Timer_Reload_1ms>>8)
#define Timer_Reload_1ms_TLx (Timer_Reload_1ms&0xff)


// ------ Public function prototypes -------------------------------


void Timer0_DELAY_1ms(uint16_t duration);



#endif