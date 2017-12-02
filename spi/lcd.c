#include"header.h"


void lcd_init()
{
	lcd_cmd(0x01);
	lcd_cmd(0x38);
	lcd_cmd(0x06);
	lcd_cmd(0x0e);
	lcd_cmd(0x80);
}

void lcd_cmd(unsigned int cmd)
{
	IO0CLR |= RS; // rs =0 for cmd mode
	IO0CLR |= RW; //read write =0 for write mode
	IO0SET |= (cmd << 15);
	IO0SET |= EN; //Enabling
	delay(1);
	IO0CLR |=EN | LCD_DATA;
	delay(5);
}


void lcd_data(unsigned char x)
{
	IO0SET |= RS; // rs =1 for data mode
	IO0CLR |= RW; //read write =0 for write mode
	IO0SET |= (x << 15);
	IO0SET |= EN; //Enabling
	delay(1);
	IO0CLR |=EN | LCD_DATA;
	delay(2);
}

void lcd_string(unsigned char *data)
{
      while(*data != '\0')
	  {
	       lcd_data(*data);
           data++;
	  }
}

void delay(unsigned int i)
{
	unsigned int j,k;
	for(j=0;j<i;j++)
		for(k=0;k<0x0000ffff;k++);
}

