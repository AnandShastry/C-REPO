#include"header.h"
char rxdata[10];
//Uart Initialization
void uart_init()
{
       U1LCR = 0x83;
	   U1DLL = (15000000)/(BAUD<<4);
	   U1LCR = 0x03;
}

// Data transmission

void transmit_data(char  data)
{
//	  while(*data != '\0')
//	  {
	         while(U1LSR & 0x20 == 0);
			      U1THR = data;

//		     data++;	  
//	  
//	  }
}

//Data receiption

char *receive_data()
{
 	  char i=0; 
       while(i<10)
	   {
	         if(U1LSR & 0x01 == 1)
			 {
			 	rxdata[i] = U1RBR;
				if(rxdata[i] == 0x0A)
				   break;

				i++;
			 }
	   	}
	   	rxdata[i]= '\0';
	    return 	rxdata;
}
 void transmit_string(char  *data)
{
	  while(*data != '\0')
	  {
	         while(U1LSR & 0x20 == 0);
			      U1THR = *data;

		     data++;	  
	  
	  }
}