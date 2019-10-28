#include "../main.h"
#include "I2C.h"
#include <stdio.h>

#define I2C_FREQUENCY 25000
#define I2C_RELOAD ((65536 - ((OSC_FREQ) / (OSC_PER_INST * I2C_FREQUENCY * 2UL))))
#define I2C_RELOAD_H (I2C_RELOAD / 256)
#define I2C_RELOAD_L (I2C_RELOAD % 256)
#define CONTINUE (1)
#define STOP (2)

uint8_t I2C_Read(uint8_t address, uint32_t int_address, uint8_t int_address_size, uint8_t num_bytes,uint8_t * ret_array)
{
	uint8_t return_value = I2C_NO_ERROR;
	uint8_t send_value, receive_value;
	uint8_t index, num_bits, send_bit, sent_bit;

	//Write start address
	if (int_address_size)
	{
		//return_value = I2C_Write(address, int_address, int_address_size, 0, ret_array);
	}

	if (return_value == I2C_NO_ERROR)
	{
		SDA = 1;
		SCL = 1;
		if ((SCL == 1) && (SDA == 1))
		{
			// Create start condition
			I2C_Clock_Start();
			printf("Address:%2.2bX\n", address);
			send_value = address << 1;
			send_value |= 0x01; // Set the LSB (R/W bit) to Read
			printf("send_value:%2.2bX\n", send_value);
			// Send START condition
			SDA = 0;
			index = 0; // For placement of elements into an array
			num_bits = 8; 

			// Send device address + R
			do
			{
				I2C_Clock_Delay(CONTINUE);
				SCL = 0;
				//SDA = 1; // Set as Input
				num_bits--;

				// Shift value to make room for next received bit
				send_bit = ((send_value>>num_bits) & 0x01);
				SDA = send_bit;

				I2C_Clock_Delay(CONTINUE);
				SCL = 1;
				while(SCL != 1);
				sent_bit = SDA;

				if(sent_bit != send_bit)
				{
					return_value = I2C_BUS_BUSY_ERROR;
				}
			} while ((num_bits > 0) && (return_value == I2C_NO_ERROR));

			if (return_value == I2C_NO_ERROR)
			{
				I2C_Clock_Delay(CONTINUE);
				SCL = 0;
				SDA = 1; // Set as Input, slave will send ACK

				I2C_Clock_Delay(CONTINUE);
				SCL = 1;
				while(SCL != 1);
				sent_bit = SDA;

				if(sent_bit != 0)
				{
					printf("1\n");
					return_value = I2C_ACK_ERROR;
				}
			}

			while((num_bytes > 0 ) && (return_value == I2C_NO_ERROR))
			{
				num_bits = 8;
				do
				{
					I2C_Clock_Delay(CONTINUE);
					SCL = 0;
					SDA = 1;
					num_bits--;
					// Shift value to make room for next received bit
					receive_value = receive_value << 1;

					I2C_Clock_Delay(CONTINUE);
					SCL = 1;
					while(SCL != 1);
					sent_bit = SDA;
					receive_value |= sent_bit;
				} while (num_bits > 0);

				*ret_array = receive_value;
				ret_array++;

				if(num_bytes == 1)
				{
					send_bit = 1; // NACK on final byte
				}
				else
				{
					send_bit = 0; // ACK for more incoming bytes
				}
				I2C_Clock_Delay(CONTINUE);
				SCL = 0;
				SDA = send_bit;

				I2C_Clock_Delay(CONTINUE);
				SCL = 1;
				while(SCL != 1);
				num_bytes--;
			}
			// Stop Condition
			if(return_value != I2C_BUS_BUSY_ERROR)
			{
				I2C_Clock_Delay(CONTINUE);
				SCL = 0;
				SDA = 0;

				I2C_Clock_Delay(CONTINUE);
				SCL = 1;
				while(SCL != 1);

				I2C_Clock_Delay(STOP);
				SDA = 1;
			}
		}
		else
		{
			return_value = I2C_BUS_BUSY_ERROR;
		}
	}
	
	return return_value;	
}

uint8_t I2C_Write(uint8_t address, uint32_t int_address, uint8_t int_address_size, uint8_t num_bytes,uint8_t * write_array)
{
	uint8_t SDA, SCL; // These dont belong here. I put them here to prevent compiler errors.
	uint8_t send_value, return_value;
	uint8_t num_bits, send_bit, sent_bit;

	SCL = 1;
	SDA = 1;
	return_value = I2C_NO_ERROR;

	int_address_size = int_address_size * 8; // Byte -> Bit Conversion
	num_bytes++; // Add device address byte

	if ((SCL == 1) && (SDA == 1))
	{
		// Create start condition
		I2C_Clock_Start();
		send_value = address << 1;
		send_value &= 0xFE; // Set Bit Zero to Write

		// Send START Condition
		SDA = 0;
		do
		{
			num_bits = 8;		
			do
			{
				I2C_Clock_Delay(CONTINUE);
				SCL = 0;
				num_bits = ((send_value >> num_bits) & 0x01);
				SDA = send_bit;

				I2C_Clock_Delay(CONTINUE);
				SCL = 1;
				while(SCL != 1);
				sent_bit = SDA;
				if(sent_bit != send_bit)
				{
					return_value = I2C_BUS_BUSY_ERROR;
				}
			}while((num_bits > 0) && (return_value == I2C_NO_ERROR));

			if (return_value == I2C_NO_ERROR)
			{
				I2C_Clock_Delay(CONTINUE);
				SCL = 0;
				SDA = 1; // Set as Input, slave will send ACK

				if(int_address_size != 0)
				{
					int_address_size -= 8;
					send_value = (int_address >> int_address_size);
				}
				else
				{
					num_bytes--;
					send_value = *(write_array);
					write_array++;
				}
				I2C_Clock_Delay(CONTINUE);
				SCL = 1;
				while(SCL != 1);
				sent_bit = SDA;
				
				if(sent_bit != 0)
				{
					return_value = I2C_ACK_ERROR;
				}
			}
		}while((num_bits != 0) && (return_value == I2C_NO_ERROR));

		// Send STOP Condition
		if(return_value != I2C_BUS_BUSY_ERROR)
		{
			I2C_Clock_Delay(CONTINUE);
			SCL = 0;
			SDA = 0;

			I2C_Clock_Delay(CONTINUE);
			SCL = 1;
			while(SCL != 1);

			I2C_Clock_Delay(STOP);
			SDA = 1;
		}
	}
	else
	{
		return_value = I2C_BUS_BUSY_ERROR;
	}
	return return_value;
}

// 
void I2C_Clock_Start()
{
	// Set Timer 0 to Mode 1 - 16b Timer
   	TMOD &= 0xF0;
   	TMOD |= 0x01;

	// Disable Interrupts
   	ET0 = 0; 
	
   	TH0 = I2C_RELOAD_H;
   	TL0 = I2C_RELOAD_L;

   	TF0 = 0; // Clear OV
   	TR0 = 1; // Start Timer
}

// Clock delay specific to I2C
void I2C_Clock_Delay(uint8_t control)
{
	if (TR0 == 1) // Wait for timeout if timer is active
	{
		while(TF0 == 0);
	}

	TR0 = 0;

	if(control == CONTINUE)
	{
		TH0 = I2C_RELOAD_H;
		TL0 = I2C_RELOAD_L;

		TF0 = 0; // Clear OV
		TR0 = 1; // Start Timer
	}
}
