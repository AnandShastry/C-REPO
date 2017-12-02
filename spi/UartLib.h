
#define BAUD 9600
#define U_SEL (0<<17)|(1<<16)|(0<<19)|(1<<18)
void uart_init(void);//Initializing Uart.
char *Receive_data(void);//receiving data from Serial port.
void transmit_data(char);//transmitting data to serial port.
