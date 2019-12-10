//Experiment 6, CpE 5160, Shaun McManus, Timothy Bain
#include <stdio.h>
#include "../Main.h"
#include "../Drivers/Port.h"
#include "../Drivers/SPI.h"
#include "../Modules/File_System.h"
#include "../Modules/Read_Sector.h"
#include "../Drivers/Outputs.h"
#include "Play_Song.h"
extern uint8_t xdata buf1[512];
extern uint8_t xdata buf2[512]; 

extern uint8_t idata SecPerClus_g;

void Play_Song(uint32_t Start_Cluster)
{
   uint16_t index1, index2;
   uint8_t buffer1, buffer2, temp8;
   uint32_t idata sector, sector_offset, cluster;
	
   printf("\r\nStarting Cluster = %lu\n\r",Start_Cluster);
	 printf("\r\nSecPerClus_g = %2.2bX\n\r", SecPerClus_g);

	 cluster=Start_Cluster;
   sector=First_Sector(Start_Cluster);
   printf("Starting Sector = %lu\n\r",sector);
	 sector_offset=0;
   buffer1=1;
   buffer2=0;
   
   Clear_P2_bit(Yellow_LED);   //YELLOWLED=ON;
   index1=0;
   
   Read_Sector((sector+sector_offset), 512, buf1);

   sector_offset++;
   Set_P2_bit(Yellow_LED);  //YELLOWLED=OFF;
   Clear_P2_bit(Amber_LED);   //AMBERLED=ON;
   index2=0;
   
   Read_Sector((sector+sector_offset), 512, buf2);
   sector_offset++;
   Set_P2_bit(Amber_LED);  //AMBERLED=OFF;
	 
   do
   {      
     do
     {
        if(DATA_REQ==0)
        {
          Clear_P2_bit(Green_LED);   //GREENLED=ON;
          Set_P1_bit(BIT_EN_bit);  //BIT_EN=1;
          SPI_Transfer(buf1[index1], &temp8);
          Clear_P1_bit(BIT_EN_bit);  //BIT_EN=0;
					Set_P2_bit(Green_LED);  //GREENLED=OFF;
					index1++;
           if(index1>511)
           {
              if(index2>511)
              {
								if(sector_offset == SecPerClus_g)
								{
									cluster = Find_Next_Clus(cluster, buf2);
									sector = First_Sector(cluster);
									sector_offset = 0;
								}
								//BIT_EN=0;              
                Clear_P2_bit(Amber_LED);   //AMBERLED=ON;
								index2=0;
				  
								Read_Sector((sector+sector_offset), 512, buf2);
								sector_offset++;
								
                Set_P2_bit(Amber_LED);  //AMBERLED=OFF;
								
              }
              buffer1=0;
              buffer2=1;
						}
       }
       else
       {
          if(index2>511)
          {
						if(sector_offset == SecPerClus_g)
								{
									cluster = Find_Next_Clus(cluster, buf2);
									sector = First_Sector(cluster);
									sector_offset = 0;
								}
						//BIT_EN=0;
            Clear_P2_bit(Amber_LED);   //AMBERLED=ON;
						index2=0;
			  
						Read_Sector((sector+sector_offset), 512, buf2);
						sector_offset++;
						
            Set_P2_bit(Amber_LED);  //AMBERLED=OFF;
          }
          else
          {
              if(index1>511)
              {
                  buffer1=0;
                  buffer2=1;
              }
          }
      }
   }while(buffer1==1);
   do
   {
      if(DATA_REQ==0)
      {
          Clear_P2_bit(Red_LED);   //REDLED=ON;
          Set_P1_bit(BIT_EN_bit);  //BIT_EN=1;
          SPI_Transfer(buf2[index2], &temp8);
          Clear_P1_bit(BIT_EN_bit);  //BIT_EN=0;
          Set_P2_bit(Red_LED);  //REDLED=OFF;
          index2++;
          if(index2>511)
          {
              if(index1>511)
              {
                if(sector_offset == SecPerClus_g)
								{
									cluster = Find_Next_Clus(cluster, buf1);
									sector = First_Sector(cluster);
									sector_offset = 0;
								}
								
								//BIT_EN=0; 
                Clear_P2_bit(Yellow_LED);   //YELLOWLED=ON;
								index1=0;
				  
								Read_Sector((sector+sector_offset), 512, buf1);
								sector_offset++;
								
                Set_P2_bit(Yellow_LED);  //YELLOWLED=OFF;
              }
              buffer2=0;
              buffer1=1;
         
           }
        }
        else
        {
           if(index1>511)
           {	
						 if(sector_offset == SecPerClus_g)
								{
									cluster = Find_Next_Clus(cluster, buf1);
									sector = First_Sector(cluster);
									sector_offset = 0;
								}
							//BIT_EN=0; 
							Clear_P2_bit(Yellow_LED);   //YELLOWLED=ON;
							index1=0;
						 
							Read_Sector((sector+sector_offset), 512, buf1);
							sector_offset++;
							
							Set_P2_bit(Yellow_LED);  //YELLOWLED=OFF;
           }
           else
           {
               if(index2>511)
               {
                  buffer2=0;
                  buffer1=1;
               }
           }
        }
      }while(buffer2==1);
  }while(cluster != 0x0FFFFFFF);//Plays until the end of file marker is detected.
} 



