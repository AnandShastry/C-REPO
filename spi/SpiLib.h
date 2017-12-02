typedef struct memory
{
	int *ptr;
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
#define SIZE 32766
#define MEMSIZE 90

// All SPI Function protrotpes

void spi_init(void);

/* read and write characters*/
void spi_write_char(unsigned char, const char , const char);
unsigned char spi_read_char(const char, const char);

/*read and write status*/
void status_write(unsigned int);
char status_read(void);

/*read and write string*/
void spi_write_string(unsigned char *, int , const char,const char);
unsigned char* spi_read_string(int,const char, const char);
void spi_read_table(void);
/*Write memory*/
void spi_write_memory(FRAM *sptr);

/*Address update in memory*/
void address_update(void);



/*Memory checking*/
void spi_write_check(void);
void spi_read_check(void);

/*To display error location*/
void ConDisp(unsigned int, unsigned char);


