#include<LPC214X.H>
#include<string.h>

#define LCD_DATA (0xff << 15)
#define EN (1 << 10)
#define RW (1 << 12)
#define RS (1 << 13)

/*delay function prototype*/
void delay(unsigned int);

/*16x2 LCD functions prototype*/
void lcd_init(void);
void lcd_cmd(unsigned int);
void lcd_data(unsigned char);
void lcd_string(unsigned char *);

/*Structure for writing the memory*/

/*spi related headers*/
/******************************************************************/
typedef struct memory
{
	int ptr;
	int size;	
}FRAM;

//Headers required
#define P_SEL (1 << 8) | (1 << 10) | (1 << 12)|(0 << 9) | (0 << 11) | (0 << 13)
#define S_SEL (1 << 7)

// Macros for all the opcodes.
#define WREN 6
#define WRITE 2
#define READ 3
#define WRSR 1
#define RDSR 5

// Memory size
#define SIZE 32765
#define MEMSIZE 90

// All SPI Function protrotpes

void spi_init(void);

/* read and write characters*/
void spi_write_char( char, const char , const char);
unsigned char spi_read_char(const char, const char);

/*read and write status*/
void status_write(unsigned int);
char status_read(void);

/*read and write string*/
void spi_write_string(char *, int);
unsigned char* spi_read_string(int,const char, const char);
void spi_read_table(void);
/*Write memory*/
int put_string(char*,int);

/*Address update in memory*/
void address_update(void);



/*Memory checking*/
void spi_write_check(void);
void spi_read_check(void);

/*To display error location*/
void ConDisp(unsigned int, unsigned char);
/*****************UART related headers*******************/
#define BAUD 9600
#define U_SEL (0<<17)|(1<<16)|(0<<19)|(1<<18)
void uart_init(void);//Initializing Uart.
char *Receive_data(void);//receiving data from Serial port.
void transmit_data(char);//transmitting data to serial port.
 void transmit_string(char  *);
 unsigned int check_location(void);
 void update_location(char ,char);
 /*-------------------array--------------------------------*/
 
 /*----------------------API for deleting/clearing the DATA----------------------------*/

void delete_ptr(void);

 /*----------------------API for update the array----------------------------*/

void update_arr(int);