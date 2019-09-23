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
	value = string[index];

	while(value != '\0')
	{
		LCD_Write_Data(value);
		Timing_delay_ms(1);
		index ++;
		value = string[index];
	}
	
	return(1);
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
	LCD_Write_Command(0x01); 
	Timing_delay_ms(10);
	LCD_Write_Command(0x06); 
	Timing_delay_ms(10);
	return(1);
}

uint8_t LCD_Write_Data(uint8_t dat)
{
	Port_writePin(LCD_RS, HIGH);
	Port_writePin(LCD_EN, HIGH);
	Port_writeBus(PORT_0, dat, 0x00);
	Port_writePin(LCD_EN, LOW);
	Port_writeBus(PORT_0, LOW_POWER_STATE, 0x00);
	
	return(1);
}

uint8_t LCD_Write_Command(uint8_t command)
{
	Port_writePin(LCD_RS, LOW);
	Port_writePin(LCD_EN, HIGH);
	Port_writeBus(PORT_0, command, 0x00);
	Port_writePin(LCD_EN, LOW);
	Port_writeBus(PORT_0, LOW_POWER_STATE, 0x00);
	
	return(1);
}
uint8_t LCD_Write(uint8_t value, uint8_t rs_value)
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
	Port_writeBus(PORT_0, value, 0x00);
	Port_writePin(LCD_EN, LOW);
	Port_writeBus(PORT_0, LOW_POWER_STATE, 0x00);
	
	return(1);
}