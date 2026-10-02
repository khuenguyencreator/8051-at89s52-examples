// Bai 6 - Lap trinh 8051 quet LED 7 thanh
// Vi du 2: quet 2 LED 7 thanh Anode chung dem tu 00 den 99
// AT89S52, thach anh 12MHz
// Doan a-g, dp cua ca 2 LED noi chung P0.0 - P0.7
// Chan chung LED hang chuc / hang don vi cap nguon qua transistor PNP, chan B noi P2.0 / P2.1 (muc 0 la bat)
#include <REGX52.H>

#define LED7  P0
#define SANG  0
#define TAT   1

sbit LED_CHUC   = P2^0;
sbit LED_DONVI  = P2^1;

// Ma LED 7 thanh Anode chung tu 0 den 9 (dp tat)
unsigned char code so[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

// Tre khoang 1ms moi don vi voi thach anh 12MHz (Keil C51)
void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

// Quet 1 vong 2 LED, mat khoang 10ms
// Thu tu chuan de khong bi bong ma: xuat ma -> bat LED -> tre -> tat LED
void Hien_Thi(unsigned char value)
{
	LED7 = so[value / 10];      // hang chuc
	LED_CHUC = SANG;
	delay_ms(5);
	LED_CHUC = TAT;

	LED7 = so[value % 10];      // hang don vi
	LED_DONVI = SANG;
	delay_ms(5);
	LED_DONVI = TAT;
}

void main(void)
{
	unsigned char dem, lan_quet;

	LED_CHUC = TAT;
	LED_DONVI = TAT;

	while (1)
	{
		for (dem = 0; dem <= 99; dem++)
		{
			// moi so giu 50 vong quet x 10ms = 500ms, khong dung delay dai de LED khong bi nhap nhay
			for (lan_quet = 0; lan_quet < 50; lan_quet++)
			{
				Hien_Thi(dem);
			}
		}
	}
}
