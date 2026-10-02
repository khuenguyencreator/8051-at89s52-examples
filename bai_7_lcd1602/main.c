// Bai 7 - Lap trinh 8051 giao tiep LCD1602 che do 8 bit
// AT89S52, thach anh 12MHz, mo phong Proteus (LM016L)
#include <REGX52.H>

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

void main(void)
{
	unsigned char count = 0;

	LCD_Init();
	LCD_GotoXY(0, 0);
	LCD_Puts("Khuenguyen 8051");

	while (1)
	{
		LCD_GotoXY(1, 0);
		LCD_Puts("Dem: ");
		LCD_Char(count / 100 + '0');        // hang tram
		LCD_Char(count / 10 % 10 + '0');    // hang chuc
		LCD_Char(count % 10 + '0');         // hang don vi
		count++;
		delay_ms(1000);
	}
}
