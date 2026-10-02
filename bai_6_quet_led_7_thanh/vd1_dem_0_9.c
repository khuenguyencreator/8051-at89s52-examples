// Bai 6 - Lap trinh 8051 quet LED 7 thanh
// Vi du 1: 1 LED 7 thanh Anode chung dem tu 0 len 9 roi dem nguoc ve 0
// AT89S52, thach anh 12MHz
// Doan a-g, dp noi P0.0 - P0.7 qua dien tro han dong, chan chung noi VCC
// (LED hut dong vao P0 nen khong can dien tro keo len)
#include <REGX52.H>

#define LED7 P0

// Ma LED 7 thanh Anode chung tu 0 den 9, dang hgfedcba (h = dp), muc 0 la sang, dp tat
unsigned char code so[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

// Tre khoang 1ms moi don vi voi thach anh 12MHz (Keil C51)
void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

void main(void)
{
	unsigned char i;

	while (1)
	{
		for (i = 0; i <= 9; i++)       // dem len 0 -> 9
		{
			LED7 = so[i];
			delay_ms(500);
		}
		for (i = 10; i > 0; i--)       // dem xuong 9 -> 0
		{
			LED7 = so[i - 1];
			delay_ms(500);
		}
	}
}
