// Bai 4 - Lap trinh 8051 GPIO bat tat LED bang nut nhan
// Bai toan 2 - Cach 1: 1 nut nhan dao trang thai LED, dung while cho nha phim
// AT89S52, thach anh 12MHz
// LED noi P2.0 (muc 0 la sang), nut nhan noi P3.7
#include <REGX52.H>

sbit LED    = P2^0;
sbit BUTTON = P3^7;

// Tre khoang 1ms moi don vi voi thach anh 12MHz (Keil C51)
void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

void main(void)
{
	LED = 1;

	while (1)
	{
		if (BUTTON == 0)
		{
			delay_ms(20);              // chong doi phim luc nhan
			if (BUTTON == 0)
			{
				while (BUTTON == 0);   // cho nha phim (chuong trinh dung o day khi giu nut)
				delay_ms(20);          // chong doi phim luc nha
				LED = !LED;
			}
		}
	}
}
