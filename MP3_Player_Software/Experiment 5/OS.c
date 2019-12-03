#include "OS.h"

void sEOS_Init(uint8_t interval_ms)
{
	// Configure Timer2 in auto-reload mode
	T2CON = 0; // Clear Timer2 Control Register 
			   // (enable a-r, timer, disable T2EX, baud rate gen, clear IR flags)

	PRELOAD = (65536 - ((OSC_FREQ * interval_ms) / (OSC_PER_INST * 1000UL)));

	RCAP2H = PRELOAD / 256;
	RCAP2L = PRELOAD % 256;

	TF2 = 0; // Clear Overflow Flag.

	// Setup Interrupts
	ET2 = 1; // Enable Timer2 Interrupt
	EA = 1; // Enable Global Interrupt

	// Start Timer2 to cause periodic Interrupts
    TR2 = 1; // Run Timer2

}

void sEOS_ISR() interrupt Timer_2_overflow using 1
{
	TF2 = 0; // Clear Timer Interrupt Flag
	
	// Perform EOS Tasks

	// Start a hardware timeout that is less than the Timer2 interval
	MP3_player_timeout_start(); // TO DO

	// Choose task based on system state
	switch(gPlayerState)
	{
		case data_idle_1:
		{
			if (DATA_REQ == 0) // Switch state to data_send_1 if DATA_REQ is active
			{
				gPlayerState = data_send_1;
			}
			break;
		}
		case data_idle_2:
		{			
			if (DATA_REQ == 0) // Switch state to data_send_2 if DATA_REQ is active
			{
				gPlayerState = data_send_2;
			}
			break;
		}
		case data_send_1:
		{
			// Loop to repeatedly send data to MP3 decoder

			while ((DATA_REQ == 0) && (TF0 == 0)) // While the data request is active
			{
				// MP3 decoder chip select active high
				BIT_EN = 1; 
				SPI_Transfer(buf1[gIndex1], &temp8);
				BIT_EN = 0;

				// Increment as each byte is sent
				gIndex1++;
				
				// When index reaches 512, the buffer is empty
				if (gIndex1 > 511)
				{
					if (gIndex2 > 511) // Guard against both buffers being empty
					{
						// Determine if the cluster is empty
						if(gNumSectors == (DrivePtr -> SecPerClus))
						{
							gPlayerState = locate_cluster_2;
						}
						else
						{
							gPlayerState = load_buf_2;
						}
					}
					else
					{
						gPlayerState = data_send_2;
					}
					TF0 = 1; // Forces Exit
				}
			}

			// If the loop exited because DATA_REQ went inactive
			if((DATA_REQ == 1) && (gPlayerState == data_send_1))
			{			
				// Check if buffer 2 is empty, otherwise, go to the data_idle state
				if(gIndex2 > 511)
				{
					if(gNumSectors == (DrivePtr -> SecPerClus))
					{
						gPlayerState = locate_cluster_2;
					}
					else
					{
						gPlayerState = load_buf_2;
					}
				}
				else
				{
					gPlayerState = update_lcd_1;
				}
			}
			break;
		}
		case data_send_2:
		{
			// Loop to repeatedly send data to MP3 decoder

			while ((DATA_REQ == 0) && (TF0 == 0)) // While the data request is active
			{
				// MP3 decoder chip select active high
				BIT_EN = 1; 
				SPI_Transfer(buf1[gIndex2], &temp8);
				BIT_EN = 0;

				// Increment as each byte is sent
				gIndex2++;
				
				// When index reaches 512, the buffer is empty
				if (gIndex2> 511)
				{
					if (gIndex1 > 511) // Guard against both buffers being empty
					{
						// Determine if the cluster is empty
						if(gNumSectors == (DrivePtr -> SecPerClus))
						{
							gPlayerState = locate_cluster_1;
						}
						else
						{
							gPlayerState = load_buf_1;
						}
					}
					else
					{
						gPlayerState = data_send_1;
					}
					TF0 = 1; // Forces Exit
				}
			}

			// If the loop exited because DATA_REQ went inactive
			if((DATA_REQ == 1) && (gPlayerState == data_send_2))
			{			
				// Check if buffer 2 is empty, otherwise, go to the data_idle state
				if(gIndex2 > 511)
				{
					if(gNumSectors == (DrivePtr -> SecPerClus))
					{
						gPlayerState = locate_cluster_1;
					}
					else
					{
						gPlayerState = load_buf_1;
					}
				}
				else
				{
					gPlayerState = update_lcd_2;
				}
			}
			break;
		}
		case load_buf_1:
		{
			// Read one sector and point to the next sector
			Read_Sector_ISR((gSector + gNumSectors), (DrivePtr -> BytesPerSec), buf1);

			gNumSectors++;

			// Reset the index to indicate a full buffer
			gIndex1 = 0;

			// Switch to the data_idle state
			gPlayerState = data_idle_2;
			break;
		}
		case load_buf_2:
		{
			// Read one sector and point to the next sector
			Read_Sector_ISR((gSector + gNumSectors), (DrivePtr -> BytesPerSec), buf2);

			gNumSectors++;

			// Reset the index to indicate a full buffer
			gIndex2 = 0;

			// Switch to the data_idle state
			gPlayerState = data_idle_1;
			break;
		}
		case locate_cluster_1:
		{
			// Use the find next cluster function and the empty buffer
			// to read the next cluster entry from the File Allocation Table
			gCluster = Find_Next_Cluster_ISR(gCluster, buf1);

			// Stop the song if the End of File Value (0x0FFFFFFF) is found
			if (gCluster != 0x0FFFFFFF)
			{
				// Calculate the first sector of the new cluster
				gSector = First_Sector_ISR(gCluster);

				// Reset the sector count to 0
				gNumSectors = 0;
				gPlayerState = data_idle_2;
			}
			else
			{
				// Use gPlayerStatus to stop the song.
				gPlayerStatus = 0;
				gPlayerState = data_idle_2;
			}
			break;
		}
		case locate_cluster_2:
		{
			// Use the find next cluster function and the empty buffer
			// to read the next cluster entry from the File Allocation Table
			gCluster = Find_Next_Cluster_ISR(gCluster, buf2);

			// Stop the song if the End of File Value (0x0FFFFFFF) is found
			if (gCluster != 0x0FFFFFFF)
			{
				// Calculate the first sector of the new cluster
				gSector = First_Sector_ISR(gCluster);

				// Reset the sector count to 0
				gNumSectors = 0;
				gPlayerState = data_idle_1;
			}
			else
			{
				// Use gPlayerStatus to stop the song.
				gPlayerStatus = 0;
				gPlayerState = data_idle_1;
			}
			break;
		}
		default:
		{
			break;
		}
	}
}