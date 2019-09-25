#include "../main.h"
#include "../Drivers/Port.h"
#include <stdio.h>
#include "../Drivers/UART.h"
#include "../Drivers/Timing.h"

#include "../Drivers/memory_test.h"
#include "print_bytes.h"
#include "../Modules/LCD.h"

uint8_t setup();
uint8_t loop();


void main()
{
	setup();
	while(1)
	{
		loop();
	}
}

uint8_t setup()
{
	Port_writePin(GREEN_LED, LOW);
	
	//Serial Init
	UART_Init_9600();
	
	//LCD Init
	LCD_Init();
	Timing_delay_ms(1000);
	Port_writePin(GREEN_LED, HIGH);	//Indicates successful init
	return(1);
}

uint8_t loop()
{
	//Laod string
	uint8_t* LCD_String = "Test String";
	
	//Write String to LCD
	LCD_Write_String(LCD_String, LINE_1);
	LCD_Write_String(LCD_String, LINE_2);
	
	//Write String to Serial
	printf(LCD_String);
	
	//Delay
	Timing_delay_ms(1000);

	return(1);
}

