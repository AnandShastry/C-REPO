#include"header.h"


/*Function : Start : spi_write_check*/
/* Added for checking the complete memory*/
/*Before any writing, status register has to written, the function spi_swrite is called before calling the below function*/
void spi_write_check()
{
	unsigned int i=0;
	IO0CLR |=S_SEL; // /CS in FRAM is given low signal by clearing the SSEL bit in mc
	S0SPDR=WREN; // "Set Write Enable Latch" opcode is provided by mc through the data register
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer
	IO0SET |=S_SEL;// /CS in FRAM is given high signal by setting the SSEL bit in mc

	delay(1);
	IO0CLR |=S_SEL;// /CS in FRAM is given low signal by clearing the SSEL bit in mc
    S0SPDR=WRITE;// "Write Memory Data" opcode is provided by mc through the data register
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer

	S0SPDR=0x00; // MSB	of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer

	S0SPDR=0x00;  // LSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer

	for(i=0;i<SIZE;i++)// loop to write the entire memory(32 kbytes) of FRAM
	{
		S0SPDR='A';	// Writing the data -- A
		while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer		
	}
	 IO0SET |=S_SEL;// /CS in FRAM is given high signal by setting the SSEL bit in mc to make the transfer complete
	 lcd_cmd(0xC0); // 2nd line in the lcd display is selected
	 lcd_string("writing done");// Display in the lcd to indicate the data write completion
	 delay(100);
}

/*Function : END : spi_write_check*/
/*-----------------------------------------------------------------------------------*/

/*Function : Start : spi_READ_check*/
/* Added for checking the complete memory*/

void spi_read_check()
{
	unsigned char ret;
	unsigned int i=0,flag=0;
	S0SPCR=0x838; // control register is configured for the CPOL=1,CPHA=1 as reading will be on the falling edge 

	IO0CLR |=S_SEL;// /CS in FRAM is given low signal by clearing the SSEL bit in mc
	S0SPDR=READ;// "Read Memory Data" opcode is provided by mc through the data register
	while(!(S0SPSR & 0x80)); // Checking for the SPIF bit to set -- completion of data transfer

	S0SPDR=0x00;  // MSB of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80)); // Checking for the SPIF bit to set -- completion of data transfer 
					   
	S0SPDR=0x00; // LSB	of address to be accessed is sent to the FRAM
	while(!(S0SPSR & 0x80));// Checking for the SPIF bit to set -- completion of data transfer

	for(i=0;i<=SIZE;i++)// loop to read the entire memory(32 kbytes) of FRAM
	{
		S0SPDR=0xEE;// Dummy byte from MC is sent to FRAMs shift register to receive the data
		while(!(S0SPSR & 0x80)); // Checking for the SPIF bit to set -- completion of data transfer
		
		ret=S0SPDR;// shifted out Content of data register from FRAM to MCs Data register. It is copied to variable 

		if(ret!='A')// If retrived value is other than what being written, execute the statements below this
		{
					
				lcd_cmd(0x01);
				lcd_string("Check Failed");
				delay(1);
				lcd_cmd(0xc0);
                ConDisp(i,ret); // Calling the function to display the read failed location in LCD
				flag++;	// used to denote the number of fails
				lcd_data(flag+48);// number of fails being displayed on LCD
				delay(200);
		}
	//	delay(1);
	}
	lcd_cmd(0x01);
	lcd_string("Memory Verified");// Memory verification status is displayed on the LCD
	IO0SET |=S_SEL;	// /CS in FRAM is given high signal by setting the SSEL bit in mc
//	 return ret;
}
/*Function : END : spi_read_check*/

/*Function : start : ConDisp*/
/*used for converting the location to asci and display on lcd*/

 void ConDisp(unsigned int value, unsigned char ret)
 {
       unsigned int temp = value;
	   unsigned int mod;
	   int i;
	   unsigned int a[5];

	   while(temp != 0)
	   {	          
			 mod = temp%10; 
	   		 temp = temp/10;
			 a[i] = mod;
			 i++;			     
	   }
	   for(i=4;i>=0;i--)
	    lcd_data(a[i]+48); 
		lcd_data(' ');
		lcd_data(ret);
 }
/*Function : END : ConDisp*/

/*---------------------------------------------------------------------*/

