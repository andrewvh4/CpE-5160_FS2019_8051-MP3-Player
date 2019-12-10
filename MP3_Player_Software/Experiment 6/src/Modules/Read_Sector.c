#include "../Main.H"
#include "../Drivers/Port.h"
#include "SDCard.h"
#include "Read_Sector.h"
#include "../Drivers/Outputs.h"





uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data)
{
	uint8_t SDtype,error_flag=No_Disk_Error;   

	SDtype=Return_SD_Card_Type();
    Clear_P1_bit(nCS0_bit);
    error_flag=SEND_COMMAND(17,(sector_number<<SDtype));
    if(error_flag==no_errors) error_flag=read_block(sector_size,array_for_data);
    Set_P1_bit(nCS0_bit);

	if(error_flag!=no_errors)
	{
       error_flag=Disk_Error;
    }
    return error_flag;
}
