#include"header.h"  
unsigned int addr_arr[10];
int main()
{
    char status,msb,lsb;
	unsigned char *str;
	int addr;
	IO0DIR |=(LCD_DATA | RS |RW | EN | S_SEL); // Direction is set as output for the pins
	PINSEL0 |= P_SEL|U_SEL; // alternative function for the p0.4,p0.5 & p0.6 is selected as SCK,MISO,MOSI resp
//	memset(addr_arr,0,sizeof(addr_arr)); //resetting array elements to zero.
	lcd_init(); // lcd is initialized
	delay(50); //delay function -- Need to be replaced with timer with interrupts
	uart_init();
	spi_init();	// SPI initialization function
	delay(1);
	status_write(0x80);// config status register function
	delay(1);
//
	status = status_read(); // Read status register function
	delay(400);
//	 // **********collect the status and write lcd display function here
	
	lcd_cmd(0x01); // clear the lcd display

    delete_ptr();

	spi_write_string("ANAND",6);

	delay(100);
	str=spi_read_string(6,0x7F,0xE9);
	lcd_cmd(0x01);
	lcd_string(str);
	delay(10);
	addr=check_location();
		addr = addr&(0xffff);
			lcd_cmd(0x01);
			 msb = (addr&(0xff00))>>8;
	
		   lsb = addr & 0xff;
		   lcd_data(msb+48);
		   lcd_data(lsb+48);
		   delay(50);

	while(1);
  
}

