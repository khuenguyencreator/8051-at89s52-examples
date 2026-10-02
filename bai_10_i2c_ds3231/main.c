// Bai 10 - Lap trinh 8051 I2C giao tiep voi DS3231
// AT89S52, thach anh 12MHz, I2C mem (bit-bang) tren P2.6 (SCL), P2.7 (SDA)
// Hien thi gio, ngay doc tu DS3231 len LCD1602 (noi day nhu bai 7)
#include <REGX52.H>
#include <intrins.h>

sbit SCL = P2^6;
sbit SDA = P2^7;

#define DS3231_ADDR   0xD0      // dia chi 7 bit 0x68 dich trai 1 bit
#define SET_TIME      1         // 1: cai gio luc khoi dong, 0: chi doc gio

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

//************************* I2C mem *********************************************************/

// Tre khoang 5us => toc do I2C xap xi 100kHz
void I2C_Delay(void)
{
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_();
}

// Start: SDA xuong 0 trong khi SCL dang o muc 1
void I2C_Start(void)
{
	SDA = 1;
	SCL = 1;
	I2C_Delay();
	SDA = 0;
	I2C_Delay();
	SCL = 0;
}

// Stop: SDA len 1 trong khi SCL dang o muc 1
void I2C_Stop(void)
{
	SDA = 0;
	SCL = 1;
	I2C_Delay();
	SDA = 1;
	I2C_Delay();
}

// Gui 1 byte, tra ve 0 neu slave ACK, 1 neu NACK
bit I2C_Write(unsigned char dat)
{
	unsigned char i;
	bit ack;

	for (i = 0; i < 8; i++)
	{
		SDA = (dat & 0x80) ? 1 : 0;    // gui bit cao truoc
		dat <<= 1;
		SCL = 1;
		I2C_Delay();
		SCL = 0;
		I2C_Delay();
	}
	SDA = 1;            // nha SDA de slave keo xuong ACK
	SCL = 1;
	I2C_Delay();
	ack = SDA;
	SCL = 0;
	return ack;
}

// Doc 1 byte, ack = 1 neu con doc tiep, 0 neu la byte cuoi
unsigned char I2C_Read(bit ack)
{
	unsigned char i, dat = 0;

	SDA = 1;            // nha SDA cho slave dieu khien
	for (i = 0; i < 8; i++)
	{
		SCL = 1;
		I2C_Delay();
		dat <<= 1;
		if (SDA) dat |= 0x01;
		SCL = 0;
		I2C_Delay();
	}
	SDA = ack ? 0 : 1;  // master ACK = keo SDA xuong 0
	SCL = 1;
	I2C_Delay();
	SCL = 0;
	SDA = 1;
	return dat;
}

//************************* DS3231 *********************************************************/

unsigned char BCD2DEC(unsigned char bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

unsigned char DEC2BCD(unsigned char dec)
{
	return ((dec / 10) << 4) | (dec % 10);
}

unsigned char hour, minute, second, date, month, year;

void DS3231_SetTime(unsigned char h, unsigned char m, unsigned char s,
                    unsigned char d, unsigned char mo, unsigned char y)
{
	I2C_Start();
	I2C_Write(DS3231_ADDR);       // ghi
	I2C_Write(0x00);              // bat dau tu thanh ghi giay
	I2C_Write(DEC2BCD(s));
	I2C_Write(DEC2BCD(m));
	I2C_Write(DEC2BCD(h));        // che do 24h
	I2C_Write(1);                 // thu trong tuan
	I2C_Write(DEC2BCD(d));
	I2C_Write(DEC2BCD(mo));
	I2C_Write(DEC2BCD(y));
	I2C_Stop();
}

void DS3231_GetTime(void)
{
	I2C_Start();
	I2C_Write(DS3231_ADDR);       // ghi dia chi thanh ghi can doc
	I2C_Write(0x00);
	I2C_Start();                  // repeated start
	I2C_Write(DS3231_ADDR | 0x01);// doc
	second = BCD2DEC(I2C_Read(1));
	minute = BCD2DEC(I2C_Read(1));
	hour   = BCD2DEC(I2C_Read(1) & 0x3F);
	I2C_Read(1);                  // bo qua thu trong tuan
	date   = BCD2DEC(I2C_Read(1));
	month  = BCD2DEC(I2C_Read(1) & 0x1F);
	year   = BCD2DEC(I2C_Read(0));// byte cuoi: NACK
	I2C_Stop();
}

// Hien thi so 2 chu so
void LCD_Num2(unsigned char n)
{
	LCD_Char(n / 10 + '0');
	LCD_Char(n % 10 + '0');
}

void main(void)
{
	LCD_Init();
#if SET_TIME
	DS3231_SetTime(9, 30, 0, 2, 10, 26);   // 09:30:00 ngay 02/10/26
#endif

	while (1)
	{
		DS3231_GetTime();

		LCD_GotoXY(0, 0);
		LCD_Puts("Gio: ");
		LCD_Num2(hour);   LCD_Char(':');
		LCD_Num2(minute); LCD_Char(':');
		LCD_Num2(second);

		LCD_GotoXY(1, 0);
		LCD_Puts("Ngay: ");
		LCD_Num2(date);   LCD_Char('/');
		LCD_Num2(month);  LCD_Char('/');
		LCD_Num2(year);

		delay_ms(200);
	}
}
