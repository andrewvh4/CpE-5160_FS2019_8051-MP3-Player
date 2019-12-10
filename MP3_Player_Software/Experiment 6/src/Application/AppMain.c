#include <stdio.h>
#include "../main.h"
#include "../Drivers/PORT.H"
#include "../Drivers/UART.h"
#include "print_bytes.h"
#include "../Drivers/SPI.h"
#include "../Modules/SDcard.h"
#include "Long_Serial_In.h"
#include "../Modules/Directory_Functions.h"
#include "../Modules/File_System.h"
#include "../Modules/STA013.h"
#include "../Modules/LCD_routines.h"
#include "../Drivers/Outputs.h"
#include "Play_Song.h"

uint8_t xdata buf1[512];
uint8_t xdata buf2[512];

extern uint32_t idata FirstRootDirSec_g;


main()
{
   uint32_t Current_directory, Entry_clus;
   uint16_t i, num_entries, entry_num;
   uint8_t error_flag;
   


   Set_P2_bit(Amber_LED);
   Set_P2_bit(Yellow_LED);
   Set_P2_bit(Green_LED);
   Clear_P2_bit(Red_LED);
   
   i=0;
   while(i<=60000) i++;
   Set_P2_bit(Red_LED);
   AUXR=0x0c;   // make all of XRAM available
   if(OSC_PER_INST==6)
   {
      CKCON0=0x01;  // set X2 clock mode
   }
   else if(OSC_PER_INST==12)
   {
      CKCON0=0x00;  // set standard clock mode
   } 
   UART_init(9600);
   printf("---LCD Init\n\r");
   LCD_Init();
   error_flag=SPI_Master_Init(400000UL);
   if(error_flag!=no_errors)
   {
      Clear_P2_bit(Red_LED);
      while(1);
   }
   LCD_Print(line1,10,"SD Card...");
   printf("---SD Card Init\n\r");
   error_flag=SD_card_init();
   if(error_flag!=no_errors)
   {
      Clear_P2_bit(Red_LED);   
      print_error(error_flag);
      while(1);
   }
   error_flag=Return_SD_Card_Type();
   if(error_flag==Standard_Capacity)
   {
      LCD_Print(no_addr_change,2,"SC");
   }
   else
   {
      LCD_Print(no_addr_change,2,"HC");
   }
   error_flag=SPI_Master_Init(20000000UL); //Set SPI to run aster after init
   if(error_flag!=no_errors)
   {
      Clear_P2_bit(Red_LED); 
      while(1);
   }
   for(i=0;i<512;i++)
   {
      buf1[i]=0xff;  // erase valout for debug
      buf2[i]=0xff;
   }
   printf("---Mounting Drive\n\r");
   LCD_Print(line2,0,"Drive...");
   error_flag=Mount_Drive(buf1);
   if(error_flag!=no_errors)
   {
      Clear_P2_bit(Red_LED);
      while(1);
   }
   LCD_Print(no_addr_change,4,"Root Found     ");
   Current_directory=FirstRootDirSec_g;
   
   
	printf("---STA Init\n\r");
    STA013_init();   
   // Main Loop

   while(1)
   {
       printf("Directory Sector = %lu\n\r",Current_directory);
       num_entries=Print_Directory(Current_directory, buf1);
	   printf("Enter Selection = ");
  	   entry_num=(uint16_t)long_serial_input();
	   if(entry_num<=num_entries)
	   {
	      Entry_clus=Read_Dir_Entry(Current_directory, entry_num, buf1);
		  if(Entry_clus & directory_bit)
		  {
		      Entry_clus & =0x0FFFFFFF;
            Current_directory=First_Sector(Entry_clus);
		  }
 	      else
		  {
		     Open_File(Entry_clus, buf1);
		  }
		  
	   }
	   else
	   {
	      printf("Invalid Selection\n\r");
	   }  
   }
} 




