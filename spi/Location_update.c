#include"header.h"

extern unsigned int addr_arr[10];

void update_location(char msb,char lsb)
{

               lcd_cmd(0x01);
			   lcd_string("location update");
			   delay(100);
			   S0SPCR=0x820; // S0SPCR config for writing to memory

                 
				IO0CLR |=S_SEL;				 // /CS in FRAM is given low signal by clearing the SSEL bit in mc
				S0SPDR=WREN;				 // // "Set Write Enable Latch" opcode to be issued before writing into memory
				while(!(S0SPSR & 0x80));
				IO0SET |=S_SEL;
///////////////// end of Write Enable//////////////////////////

 	 		  	delay(1);
				IO0CLR |=S_SEL;		        
				S0SPDR=WRITE;				// "Write Memory Data" opcode is provided by mc through the data register
				while(!(S0SPSR & 0x80));

				S0SPDR=0x7F;			    // MSB of address to be accessed is sent to the FRAM
				while(!(S0SPSR & 0x80));

				S0SPDR=0xFE;				// LSB of address to be accessed is sent to the FRAM
				while(!(S0SPSR & 0x80));

	/*updating the last written location begins here*/

				S0SPDR=msb; // Write the MSB data				
				while(!(S0SPSR & 0x80));
				S0SPDR=lsb;	//Write the LSB data			
				while(!(S0SPSR & 0x80));
				delay(1);
				IO0SET |=S_SEL;

				 lcd_cmd(0x01); 
			     lcd_data(msb+48);
				 lcd_data(lsb+48);
				 delay(10);
 				
}


unsigned int check_location()
{
		
	unsigned char msb=0,lsb=0;
	unsigned int val=0;
	S0SPCR=0x838; // S0SPCR config for reading the memory

	IO0CLR |=S_SEL;
	S0SPDR=READ;			  // Read Opcode.
	while(!(S0SPSR & 0x80));

	S0SPDR=0x7F;			  // MSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	S0SPDR=0xFE;			  // LSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	S0SPDR=0xFF;	  // dummy byte
	while(!(S0SPSR & 0x80));
	msb = S0SPDR;

	S0SPDR=0xFF;	  // dummy byte
	while(!(S0SPSR & 0x80));
	lsb = S0SPDR;
	IO0SET |=S_SEL; 
/* written below for sending the data at one shot*/		  
	   if(!((msb==0) && (lsb==0)))
	   {
	   		val=((msb&0xffff)<<8) | lsb;
			val = val & 0x0000ffff;
			return val;
			 	
	   }
	  else
	   {
	  		return 0x0000;
	   } 	
				
}


/*==============added for clearing the pointer locations================*/

void delete_ptr(void)
{

	unsigned char i;
	for(i=0;i<22;i++)
	{
	spi_write_char(0x00,0x7F,0xE9+i);	
	}

}

/*==============added for updating the array locations================*/

void update_arr(int val)
{
	unsigned char i;
	unsigned char msb=0,lsb=0,val1=0;
	S0SPCR=0x838; // S0SPCR config for reading the memory

	IO0CLR |=S_SEL;
	S0SPDR=READ;			  // Read Opcode.
	while(!(S0SPSR & 0x80));

	S0SPDR=0x7F;			  // MSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	S0SPDR=0xE9;			  // LSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	  for(i=0;i<10;i++)
	  {
		S0SPDR=0xFF;	  // dummy byte
		while(!(S0SPSR & 0x80));
		  msb = S0SPDR;

		S0SPDR=0xFF;	  // dummy byte
		while(!(S0SPSR & 0x80));
		  lsb = S0SPDR;

		  if((msb|lsb)==0x00)
		  {
		  	 addr_arr[i]=val;
			 break;
		  }
		}
		IO0SET |=S_SEL; 
		val1=(val&0x0000ff00) >> 8;
		spi_write_char(val1,msb,lsb);					
		val1=(val&0x000000ff);
		spi_write_char(val1,msb,lsb+1);
}


