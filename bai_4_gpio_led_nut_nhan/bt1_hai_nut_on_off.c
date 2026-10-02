// Bai 4 - Lap trinh 8051 GPIO bat tat LED bang nut nhan
// Bai toan 1: 2 nut nhan, nhan ON thi LED sang, nhan OFF thi LED tat
// AT89S52, thach anh 12MHz
// LED noi P2.0 (muc 0 la sang), nut ON noi P3.7, nut OFF noi P3.6
#include <REGX52.H>

sbit LED     = P2^0;
sbit BTN_ON  = P3^7;
sbit BTN_OFF = P3^6;

// Tre khoang 1ms moi don vi voi thach anh 12MHz (Keil C51)
void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

void main(void)
{
	LED = 1;           // ban dau LED tat

	while (1)
	{
		if (BTN_ON == 0)
		{
			delay_ms(20);              // cho het doi phim
			if (BTN_ON == 0)           // doc lai de chac chan nut dang duoc nhan
			{
				LED = 0;               // LED sang
			}
		}
		if (BTN_OFF == 0)
		{
			delay_ms(20);
			if (BTN_OFF == 0)
			{
				LED = 1;               // LED tat
			}
		}
	}
}
