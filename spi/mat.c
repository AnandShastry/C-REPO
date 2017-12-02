#include<LPC214X.H>
#define S_SEL (1 << 7)
#define P_SEL (1 << 8) | (1 << 10) | (1 << 12)|(0 << 9) | (0 << 11) | (0 << 13)
#define SCK   (1<<4)
#define MOSI  (1<<6)
#define MISO  (1<<5)
#define LED   (0xf<<10)
void delay(int);
void write_seg(int);

int main()
{
   PINSEL0 |= P_SEL;
   IO0DIR |=  S_SEL;
   IO0SET |=  S_SEL;
   IO0DIR |=  LED;
   S0SPCR = (0X0024);
   S0SPCCR = 0X64;
  
   
 
    IO0SET |= LED;
	delay(10); 
//   if((S0SPSR & 0x80))
//   {	
   		write_seg(0x0C01);  // shut down
		delay(1);					 
//		write_seg(0x0C00);
		
//		write_seg(0x090F);  //  decoder
//		delay(1);
		write_seg(0x0B05); // Scan limit
		delay(1);
		write_seg(0x0A0f); // Inten
		delay(1);
	
//		write_seg(0x0F00);   		
//     	delay(1);
  		
while(1)
{
		write_seg(0x013f);
		delay(1);
	  
		write_seg(0x0248);
		delay(1);
		write_seg(0x0348);
		delay(1);
		write_seg(0x0448);
		delay(1);
		write_seg(0x053f);
		delay(1);
 }
//   }
   IO0CLR |= LED;
   delay(10); 

 //  while(1);
}

void delay(int i)
{
      unsigned int j,k;
	  for(j=0;j<i;j++)
	  for(k=0;k<0xFFFFF;k++);

}

void write_seg(int value)
{
	IO0CLR |= S_SEL;					 //select slave
   	S0SPDR = value ;					 //select shutdown mode
   		while(!(S0SPSR&0x80));			 //Check for SPIF in S0SPSR
   	IO0SET |= S_SEL;
}
