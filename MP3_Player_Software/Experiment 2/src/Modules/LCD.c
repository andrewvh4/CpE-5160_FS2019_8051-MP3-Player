#include "LCD.h"
#include "../Drivers/Timing.h"
#include "../Drivers/UART.h"
#include "../Drivers/Port.h"
#include <stdio.h>

uint8_t LCD_Write_String(uint8_t* string, uint8_t line) 
{
	uint8_t index, value;
	
	LCD_Write_Command(line); //Set address to start of the line
	Timing_delay_ms(1);
	index = 0;
	value = string[index];	//Load Initial Value

	while(value != '\0')	//Loop until null termination
	{
		LCD_Write_Data(value);	//Write character
		Timing_delay_ms(1);
		index ++;
		value = string[index];	//Get next value
	}
	
	return(0);
}

uint8_t LCD_Init()
{
	Timing_delay_ms(50); //Delay after power up
	LCD_Write_Command(0x38);
	Timing_delay_ms(10); //Delay greater than 4.1ms
	LCD_Write_Command(0x38); 
	Timing_delay_ms(10); //Delay greater than 100us 
	LCD_Write_Command(0x38);
	Timing_delay_ms(10);
	LCD_Write_Command(0x38);
	Timing_delay_ms(10);
	LCD_Write_Command(0x0C); //Display on, cusor off
	Timing_delay_ms(10);
	LCD_Write_Command(0x01); //Display Clear
	Timing_delay_ms(10);
	LCD_Write_Command(0x06); //Entry mode set, increment
	Timing_delay_ms(10);
	return(0);
}

uint8_t LCD_Write_Data(uint8_t dat)
{
	LCD_Write(dat, DATA); //Write as Data
	return(0);
}

uint8_t LCD_Write_Command(uint8_t command)
{
	LCD_Write(command, COMMAND); //Write as Command
	return(0);
}
uint8_t LCD_Write(uint8_t value, uint8_t rs_value) //Generic LCD Write command
{
	if(rs_value == COMMAND)
	{
		Port_writePin(LCD_RS, LOW);
	}
	else
	{
		Port_writePin(LCD_RS, HIGH);
	}
	Port_writePin(LCD_EN, HIGH);					
	Port_writeBus(PORT_0, value, 0x00);		//Send data
	Port_writePin(LCD_EN, LOW);						//Latch Data
	Port_writeBus(PORT_0, LOW_POWER_STATE, 0x00);
	
	return(0);
}