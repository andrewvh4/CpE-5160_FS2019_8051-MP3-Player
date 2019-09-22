#include "LCD.h"
#include "../Drivers/Timing.h"
#include "../Drivers/UART.h"
#include "../Drivers/Port.h"

uint8_t LCD_Write_String(uint8_t* string, uint8_t line) 
{
	uint8_t index, value;
	
	LCD_Write_Command(line); //Set address to start of the line
	Timing_delay_ms(1);
	index = 0;
	value = string[index];

	while(value != '0')
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
	Timing_delay_ms(30); //Delay after power up
	LCD_Write_Command(0x38);
	Timing_delay_ms(5); //Delay greater than 4.1ms
	LCD_Write_Command(0x38); 
	Timing_delay_ms(1); //Delay greater than 100us 
	LCD_Write_Command(0x38);
	Timing_delay_ms(1);
	LCD_Write_Command(0x38);
	Timing_delay_ms(1);
	LCD_Write_Command(0x0C); //Display on, cusor off
	Timing_delay_ms(1);
	return(1);
}

uint8_t LCD_Write_Data(uint8_t dat)
{
	Port_writePin(LCD_RS, HIGH);
	Port_writePin(LCD_EN, HIGH);
	Port_writeBus(PORT_0, 0x00, dat);
	Port_writePin(LCD_EN, LOW);
	Port_writeBus(PORT_0, 0x00, LOW_POWER_STATE);
	
	return(1);
}

uint8_t LCD_Write_Command(uint8_t command)
{
	Port_writePin(LCD_RS, LOW);
	Port_writePin(LCD_EN, HIGH);
	Port_writeBus(PORT_0, 0x00, command);
	Port_writePin(LCD_EN, LOW);
	Port_writeBus(PORT_0, 0x00, LOW_POWER_STATE);
	
	return(1);
}
uint8_t LCD_Write(uint8_t value, uint8_t rs_value)
{
	if(rs_value == COMMMAND)
	{
		Port_writePin(LCD_RS, LOW);
	}
	else
	{
		Port_writePin(LCD_RS, HIGH);
	}
	Port_writePin(LCD_EN, HIGH);
	Port_writeBus(PORT_0, 0x00, value);
	Port_writePin(LCD_EN, LOW);
	Port_writeBus(PORT_0, 0x00, LOW_POWER_STATE);
	
	return(1);
}