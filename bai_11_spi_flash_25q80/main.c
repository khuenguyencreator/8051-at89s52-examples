// Bai 11 - Lap trinh 8051 SPI giao tiep voi IC Flash W25Q80 (25Q80)
// AT89S52, thach anh 12MHz, SPI mem (bit-bang) mode 0
// Doc JEDEC ID, xoa sector 0, ghi chuoi roi doc lai, hien thi len LCD1602 (noi day nhu bai 7)
#include <REGX52.H>

sbit FLASH_CS   = P1^0;
sbit FLASH_SCK  = P1^1;
sbit FLASH_MOSI = P1^2;
sbit FLASH_MISO = P1^3;

#define CMD_WRITE_ENABLE   0x06
#define CMD_READ_STATUS1   0x05
#define CMD_PAGE_PROGRAM   0x02
#define CMD_READ_DATA      0x03
#define CMD_SECTOR_ERASE   0x20
#define CMD_JEDEC_ID       0x9F

#define LCD_DATA P0       // D0-D7 cua LCD noi voi P0 (can dien tro keo len)
sbit LCD_RS = P2^0;
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;

// Tre khoang 1ms moi don vi voi thach anh 12MHz (Keil C51)
void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

// Tao xung tren chan EN de LCD chot du lieu
void LCD_Enable(void)
{
	LCD_EN = 1;
	delay_ms(1);
	LCD_EN = 0;
	delay_ms(1);
}

// Gui lenh: RS = 0
void LCD_Cmd(unsigned char cmd)
{
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_DATA = cmd;
	LCD_Enable();
}

// Gui ky tu: RS = 1
void LCD_Char(unsigned char c)
{
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_DATA = c;
	LCD_Enable();
}

void LCD_Init(void)
{
	delay_ms(20);      // cho LCD on dinh nguon
	LCD_Cmd(0x38);     // giao tiep 8 bit, 2 dong, font 5x8
	LCD_Cmd(0x0C);     // bat hien thi, tat con tro
	LCD_Cmd(0x06);     // tu dong tang dia chi sau moi ky tu
	LCD_Cmd(0x01);     // xoa man hinh
	delay_ms(2);
}

// row: 0 hoac 1, col: 0 - 15
void LCD_GotoXY(unsigned char row, unsigned char col)
{
	if (row == 0)
		LCD_Cmd(0x80 + col);
	else
		LCD_Cmd(0xC0 + col);
}

void LCD_Puts(char *s)
{
	while (*s)
	{
		LCD_Char(*s++);
	}
}

//************************* SPI mem *********************************************************/

// Gui va nhan dong thoi 1 byte, SPI mode 0 (CPOL = 0, CPHA = 0), bit cao truoc
unsigned char SPI_Transfer(unsigned char dat)
{
	unsigned char i, rx = 0;

	for (i = 0; i < 8; i++)
	{
		FLASH_MOSI = (dat & 0x80) ? 1 : 0;   // dat bit len MOSI khi SCK = 0
		dat <<= 1;
		FLASH_SCK = 1;                       // slave doc MOSI o canh len
		rx <<= 1;
		if (FLASH_MISO) rx |= 0x01;          // master doc MISO o canh len
		FLASH_SCK = 0;
	}
	return rx;
}

//************************* W25Q80 *********************************************************/

void Flash_WaitBusy(void)
{
	FLASH_CS = 0;
	SPI_Transfer(CMD_READ_STATUS1);
	while (SPI_Transfer(0xFF) & 0x01);       // bit BUSY = 1 la dang ban
	FLASH_CS = 1;
}

void Flash_WriteEnable(void)
{
	FLASH_CS = 0;
	SPI_Transfer(CMD_WRITE_ENABLE);
	FLASH_CS = 1;
}

void Flash_ReadID(unsigned char *id)
{
	FLASH_CS = 0;
	SPI_Transfer(CMD_JEDEC_ID);
	id[0] = SPI_Transfer(0xFF);              // ma nha san xuat
	id[1] = SPI_Transfer(0xFF);              // loai bo nho
	id[2] = SPI_Transfer(0xFF);              // dung luong
	FLASH_CS = 1;
}

// Xoa 1 sector 4KB chua dia chi addr
void Flash_SectorErase(unsigned long addr)
{
	Flash_WriteEnable();
	FLASH_CS = 0;
	SPI_Transfer(CMD_SECTOR_ERASE);
	SPI_Transfer(addr >> 16);
	SPI_Transfer(addr >> 8);
	SPI_Transfer(addr);
	FLASH_CS = 1;
	Flash_WaitBusy();
}

// Ghi toi da 256 byte, khong duoc vuot qua bien cua 1 page
void Flash_PageWrite(unsigned long addr, unsigned char *buf, unsigned int len)
{
	Flash_WriteEnable();
	FLASH_CS = 0;
	SPI_Transfer(CMD_PAGE_PROGRAM);
	SPI_Transfer(addr >> 16);
	SPI_Transfer(addr >> 8);
	SPI_Transfer(addr);
	while (len--)
	{
		SPI_Transfer(*buf++);
	}
	FLASH_CS = 1;
	Flash_WaitBusy();
}

void Flash_Read(unsigned long addr, unsigned char *buf, unsigned int len)
{
	FLASH_CS = 0;
	SPI_Transfer(CMD_READ_DATA);
	SPI_Transfer(addr >> 16);
	SPI_Transfer(addr >> 8);
	SPI_Transfer(addr);
	while (len--)
	{
		*buf++ = SPI_Transfer(0xFF);
	}
	FLASH_CS = 1;
}

// Hien thi 1 byte dang so hex 2 ky tu
void LCD_Hex(unsigned char n)
{
	unsigned char code hex[] = "0123456789ABCDEF";
	LCD_Char(hex[n >> 4]);
	LCD_Char(hex[n & 0x0F]);
}

unsigned char id[3];
unsigned char write_buf[] = "8051 SPI OK";
unsigned char read_buf[12];

void main(void)
{
	FLASH_CS = 1;
	FLASH_SCK = 0;
	LCD_Init();

	// 1. Doc JEDEC ID
	Flash_ReadID(id);
	LCD_GotoXY(0, 0);
	LCD_Puts("ID: ");
	LCD_Hex(id[0]);
	LCD_Hex(id[1]);
	LCD_Hex(id[2]);

	// 2. Xoa sector 0, ghi chuoi vao dia chi 0, doc lai
	Flash_SectorErase(0x000000);
	Flash_PageWrite(0x000000, write_buf, sizeof(write_buf));
	Flash_Read(0x000000, read_buf, sizeof(write_buf));

	LCD_GotoXY(1, 0);
	LCD_Puts((char *)read_buf);

	while (1)
	{
	}
}
