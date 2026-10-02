// Bai 14 - Lap trinh 8051 giao tiep 1-Wire voi cam bien nhiet do DS18B20
// AT89S52, thach anh 12MHz, DQ noi P3.7 (dien tro keo len 4.7K)
// Hien thi nhiet do len LCD1602 (noi day nhu bai 7)
#include <REGX52.H>
#include <intrins.h>

sbit DQ = P3^7;

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

//************************* 1-Wire *********************************************************/

// Tre khoang 2*t + 5 us voi thach anh 12MHz (Keil C51 dich while(--t) thanh lenh DJNZ 2 chu ky)
void delay_us(unsigned char t)
{
	while (--t);
}

// Xung reset va kiem tra xung presence. Tra ve 1 neu co cam bien tra loi
bit OW_Reset(void)
{
	bit presence;

	DQ = 0;
	delay_us(250);      // giu muc 0 khoang 500us (toi thieu 480us)
	DQ = 1;
	delay_us(35);       // cho khoang 70us roi doc
	presence = !DQ;     // DS18B20 keo DQ xuong 0 neu co mat
	delay_us(220);      // cho het khe presence
	return presence;
}

void OW_WriteBit(bit b)
{
	DQ = 0;
	_nop_(); _nop_();   // giu muc 0 khoang 2us
	if (b) DQ = 1;      // bit 1: nha bus ngay
	delay_us(30);       // ca khe thoi gian khoang 65us
	DQ = 1;
}

bit OW_ReadBit(void)
{
	bit b;

	DQ = 0;
	_nop_(); _nop_();   // keo xuong 2us de bat dau khe doc
	DQ = 1;             // nha bus cho cam bien dieu khien
	_nop_(); _nop_(); _nop_(); _nop_();
	b = DQ;             // doc trong vong 15us dau
	delay_us(25);       // cho het khe doc
	return b;
}

void OW_WriteByte(unsigned char dat)
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		OW_WriteBit(dat & 0x01);   // bit thap gui truoc
		dat >>= 1;
	}
}

unsigned char OW_ReadByte(void)
{
	unsigned char i, dat = 0;
	for (i = 0; i < 8; i++)
	{
		dat >>= 1;
		if (OW_ReadBit()) dat |= 0x80;
	}
	return dat;
}

//************************* DS18B20 *********************************************************/

// Tra ve nhiet do theo don vi 1/16 do C, tra ve 0x7FFF neu khong co cam bien
int DS18B20_ReadRaw(void)
{
	unsigned char lsb, msb;

	if (!OW_Reset()) return 0x7FFF;
	OW_WriteByte(0xCC);          // Skip ROM: chi co 1 cam bien tren bus
	OW_WriteByte(0x44);          // Convert T: bat dau do
	delay_ms(750);               // do phan giai 12 bit can toi da 750ms

	OW_Reset();
	OW_WriteByte(0xCC);
	OW_WriteByte(0xBE);          // Read Scratchpad
	lsb = OW_ReadByte();
	msb = OW_ReadByte();
	return ((int)msb << 8) | lsb;
}

void main(void)
{
	int raw;
	unsigned int temp10;         // nhiet do x10, vi du 25.6 do -> 256

	LCD_Init();
	LCD_GotoXY(0, 0);
	LCD_Puts("Nhiet do DS18B20");

	while (1)
	{
		raw = DS18B20_ReadRaw();
		LCD_GotoXY(1, 0);

		if (raw == 0x7FFF)
		{
			LCD_Puts("Loi cam bien   ");
			continue;
		}
		if (raw < 0)
		{
			LCD_Char('-');
			raw = -raw;
		}
		else
		{
			LCD_Char(' ');
		}
		temp10 = ((unsigned long)raw * 10) / 16;
		LCD_Char(temp10 / 1000 + '0');
		LCD_Char(temp10 / 100 % 10 + '0');
		LCD_Char(temp10 / 10 % 10 + '0');
		LCD_Char('.');
		LCD_Char(temp10 % 10 + '0');
		LCD_Char(0xDF);              // ky tu do trong bang ma LCD
		LCD_Puts("C      ");
	}
}
