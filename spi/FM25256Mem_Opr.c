#include"header.h"

/*Function : start : SPI initialization function*/
//extern FRAM snode;
extern int size;
extern int address;
unsigned char ret[20];
void spi_init()
{
	S0SPCR =0X0820; // Control register is being loaded with value to make CPOL=0,CPHA=0.MSTR=1 as data write starts in the prior raising edge of clock
	S0SPCCR =0X64; //Clock control register is loaded with the divisor value, which yiels .012Mhz PCLK
	delay(10);
}

/*Function : END : SPI initialization function*/

/*----------------------------------------------------------------------------------*/
/*Function : start : SPI WRITE configuration function*/
void status_write(unsigned int data)
{
///////////// opcode for Set Write Enable Latch (WEL)=1  ////////////////////	    
		IO0CLR |=S_SEL;	// /CS in FRAM is given low signal by clearing the SSEL bit in mc
  	    S0SPDR=WREN;  // opcode for Set Write Enable Latch
		while(!(S0SPSR & 0x80)); // Checking for the SPIF bit to set -- completion of data transfer
	    IO0SET |=S_SEL;// /CS in FRAM is given high signal by setting the SSEL bit in mc
        delay(1);
/////////    // Write Status Register (WPEN=1)/////////////////////////
	 	IO0CLR |=S_SEL; // /CS in FRAM is given low signal by clearing the SSEL bit in mc
		S0SPDR=WRSR; // "Write Status Register" opcode is written
		while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer

		S0SPDR=data;// status reg Config, WPEN=1, BP0=BP1=0
		while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer
	 	IO0SET |=S_SEL;// /CS in FRAM is given high signal by setting the SSEL bit in mc

}

/*Function : start : Read status register function*/

 char status_read()
 {
 	unsigned char rd,i;
 	char ret;
	IO0CLR |=S_SEL;	// /CS in FRAM is given low signal by clearing the SSEL bit in mc
	S0SPDR=RDSR;//"Read Status Register" opcode is being sent to the data register
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer

	S0SPDR=0x00;// Dummy byte from MC is sent to FRAMs shift register to receive the data
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer
	rd=S0SPDR;//shifted out Content of data register from FRAM to MCs Data register. It is copied to variable 
	IO0SET |=S_SEL;	// /CS in FRAM is given high signal by setting the SSEL bit in mc
	
	ret = rd;
	
	
	for(i=0;i<=7;i++) // loop to display the status register value in binary format in LCD
	{		
		if(rd & 0x80)
			lcd_data('1');
		else
			lcd_data('0');

		rd=(rd << 1);		
	}
	return ret;
 }
 /*Function : END : Read status register function*/


/*Function : start : SPI write function*/
/* below function is used to write the data into FRAM */

void spi_write_char( char val,const char msb, const char lsb)
{
	IO0CLR |=S_SEL;// /CS in FRAM is given low signal by clearing the SSEL bit in mc
	S0SPDR=WREN; // "Set Write Enable Latch" opcode to be issued before writing into memory
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer
	IO0SET |=S_SEL;// /CS in FRAM is given low signal by clearing the SSEL bit in mc

	delay(1);
	IO0CLR |=S_SEL;// /CS in FRAM is given high signal by setting the SSEL bit in mc
	S0SPDR=WRITE; // "Write Memory Data" opcode is provided by mc through the data register
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer

	S0SPDR=msb;  // MSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer

	S0SPDR=lsb;// LSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80)); // Checking for the SPIF bit to set -- completion of data transfer

	S0SPDR=val; // value to bewritten is given to SPI data register
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer

	 IO0SET |=S_SEL;// /CS in FRAM is given high signal by setting the SSEL bit in mc
}
/*Function : END : SPI write function*/


unsigned char spi_read_char(const char msb, const char lsb)
{
	unsigned char ret;

	S0SPCR=0x838;

	IO0CLR |=S_SEL;   // /CS in FRAM is given low signal by clearing the SSEL bit in mc
	S0SPDR=READ;	  // Read Opcode.
	while(!(S0SPSR & 0x80));

	S0SPDR=msb;	  // MSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	S0SPDR=lsb;	  // LSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	S0SPDR=0x00;	  // Dummy data to read the data from slave(FRAM)
	while(!(S0SPSR & 0x80));

	IO0SET |=S_SEL; // /CS in FRAM is given high signal by setting the SSEL bit in mc

	ret=S0SPDR;	 //  Assigning the received data to a variable
	return ret;	 
}


/*-------------Write and read string functions--------------*/

void spi_write_string(char* str, int size)	
{
			  
    unsigned int addr;
	unsigned char msb,lsb;
	unsigned int val = 0x00; // temporary variable to be used as location pointer


	addr=check_location();
	addr=addr&(0xffff);
	lcd_cmd(0x01);

	delay(100);
    	 
	if((addr&0x7FFE) >= 0x7FFE)
	{
		  transmit_string("ERR:0");
		   return;	      
	}
	else
	{
		   msb = (addr&(0xff00)>>8);
		 
		   lsb = addr & 0xff;
		   lcd_data(msb+48);
		   lcd_data(lsb+48);
		   delay(50);
	}
		 	
	
	
	  S0SPCR=0x820; // S0SPCR config for writing to memory
    ///////////// opcode for Set Write Enable Latch (WEL)=1  ////////////////////	  

	IO0CLR |=S_SEL;				 // /CS in FRAM is given low signal by clearing the SSEL bit in mc
	S0SPDR=WREN;				 // // "Set Write Enable Latch" opcode to be issued before writing into memory
	while(!(S0SPSR & 0x80));
	IO0SET |=S_SEL;
///////////////// end of Write Enable//////////////////////////

    delay(1);
	IO0CLR |=S_SEL;		        
	S0SPDR=WRITE;				// "Write Memory Data" opcode is provided by mc through the data register
	while(!(S0SPSR & 0x80));

	S0SPDR=msb;			    // MSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	S0SPDR=lsb;				// LSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

  /////////////////Sending character by character////////////////////
	while(*str!='\0')
	{
		S0SPDR=*str;
		while(!(S0SPSR & 0x80));
		str++;
		val++;
	}
	if(*str == '\0')
	{
		 S0SPDR= 0x1A;
		 while(!(S0SPSR & 0x80));
		 val++;	
	}
	 IO0SET |=S_SEL;
	 val = val+addr;
	 msb = (val&(0xff00))>>8;
	 lsb = val & 0xff;
	 update_location(msb,lsb);
	 update_arr(val);
}



unsigned char* spi_read_string(int size,const char msb, const char lsb)
{
	

	unsigned int i=0;
	unsigned char msb1,lsb1=0;
	S0SPCR=0x838;

	IO0CLR |=S_SEL;
	S0SPDR=READ;			  // Read Opcode.
	while(!(S0SPSR & 0x80));

	S0SPDR=msb;			  // MSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	S0SPDR=lsb;			  // LSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	S0SPDR=0xFF;	  // dummy byte
	while(!(S0SPSR & 0x80));
	msb1=S0SPDR;

	S0SPDR=0xFF;	  // dummy byte
	while(!(S0SPSR & 0x80));
	lsb1=S0SPDR;
	IO0SET |=S_SEL;
	 delay(50);
	////////////////////////////reading all characters upto null/////////////////
	S0SPCR=0x838;

	IO0CLR |=S_SEL;
	S0SPDR=READ;			  // Read Opcode.
	while(!(S0SPSR & 0x80));

	S0SPDR=msb1;			  // MSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	S0SPDR=lsb1;			  // LSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));

	while(i<size)
	{
		S0SPDR=0xFF;	  // dummy byte
		while(!(S0SPSR & 0x80));		   
		*(ret+i)=S0SPDR;

		if(*(ret+i) == '\0')
		   break;
		i++;
	}

	ret[size]='\0';
	IO0SET |=S_SEL;
//	*ret=S0SPDR;
	return ret;
}

//int put_string(char *str, int size)
//{
//	/* Note:- Here i is address where data address is stored*/
//	/* And j is the location of the data*/
//	
//	
//	int j=0x14;
//	char value,msb,lsb,i=0x00;
//	char msb1, lsb1;
//	S0SPCR=0x838;
//
//	IO0CLR |=S_SEL;
//	S0SPDR=READ;			  // Read Opcode.
//	while(!(S0SPSR & 0x80));
//
//	S0SPDR=0x00;			  // MSB of address to be accessed is sent to the FRAM
//	while(!(S0SPSR & 0x80));
//
//	S0SPDR=0x00;			  // LSB of address to be accessed is sent to the FRAM
//	while(!(S0SPSR & 0x80));
//
//	////////////////////////////reading all characters upto null/////////////////
//	while(i<0x14)
//	{
//		S0SPDR=0x00;	  // dummy byte
//		while(!(S0SPSR & 0x80));		   
//		value=S0SPDR;
//
//		if(value == 'A')
//		   break;
//		 i++;
//	}
//	
//	if(i<0x14)
//	{
//		while(j<0x7FF0)
//		{
//				 j++;
//			S0SPDR=0x00;	  // dummy byte
//			while(!(S0SPSR & 0x80));		   
//				value=S0SPDR;
//
//			if(value == 0x26)
//				   break;
//		
//		}
//		if((j<0x7FF0) && ((0x7FF0-j)>= size)) 	
//		{
//			   IO0SET |=S_SEL;
//			   msb = j&(0xff00);
//			   msb = msb>>8;
//			   lsb = j & (0xff);
//			   spi_write_string(str,size,msb,lsb);
//			   IO0SET |=S_SEL;
//
//			   msb1 = i&(0xff00);
//			   msb1 = msb1>>8;
//			   lsb1 = i & (0xff);
//			   spi_write_char(msb,msb1,lsb1);
//			   i= i+1;
//			   msb1 = i&(0xff00);
//			   msb1 = msb1>>8;
//			   lsb1 = i & (0xff);
//			   spi_write_char(lsb,msb1,lsb1);
//		}
//		else
//		{
//			    IO0SET |=S_SEL;
//				transmit_string("ERR:0");
//		
//		}
//	
//	}
//	else
//	{
//		 IO0SET |=S_SEL;
//		 transmit_string("ERR:1");
//	}
//
//
//	return (0x00+i);
//		
//
//}
