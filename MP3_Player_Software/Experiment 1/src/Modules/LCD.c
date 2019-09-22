#include "LCD.h"
#include "../Drivers/Timing.h"
#include "../drivers/UART.h"

uint8_t LCD_Write_String(uint8_t* string, uint8_t line, uint8_t starting_char)
{
	//Call Write Data
	return(1);
}

uint8_t LCD_Init()
{
	return(1);
}

uint8_t LCD_Write_Data(uint8_t dat)
{
	Port_writePin(LCD_RS, HIGH);
	Port_writePin(LCD_EN, HIGH);
	Port_writePort(PORT_0, 0x00, dat);
	Port_writePin(LCD_EN, LOW);
	Port_writePort(PORT_0, 0x00, LOW_POWER_STATE);
	return(1);
}

uint8_t LCD_Write_Command(uint8_t command)
{
	Port_writePin(LCD_RS, LOW);
	Port_writePin(LCD_EN, HIGH);
	Port_writePort(PORT_0, 0x00, command);
	Port_writePin(LCD_EN, LOW);
	Port_writePort(PORT_0, 0x00, LOW_POWER_STATE);
	
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
	Port_writePort(PORT_0, 0x00, value);
	Port_writePin(LCD_EN, LOW);
	Port_writePort(PORT_0, 0x00, LOW_POWER_STATE);
}