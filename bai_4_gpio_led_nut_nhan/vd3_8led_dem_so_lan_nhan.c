// Bai 4 - Lap trinh 8051 GPIO bat tat LED bang nut nhan
// Vi du 3: nhan BT1 lan 1 thi 8 LED sang, nhan lan 2 thi 8 LED tat, cu the lap lai
// AT89S52, thach anh 12MHz
// 8 LED noi P2.0 - P2.7 (muc 0 la sang), BT1 noi P1.0
#include <REGX52.H>

#define LEDS P2

sbit BT1 = P1^0;

unsigned char press_count = 0;   // so lan nhan, chi nhan gia tri 1 hoac 2

// Tre khoang 1ms moi don vi voi thach anh 12MHz (Keil C51)
void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

void main(void)
{
	LEDS = 0xFF;           // ban dau tat het 8 LED

	while (1)
	{
		if (BT1 == 0)
		{
			delay_ms(20);
			if (BT1 == 0)
			{
				while (BT1 == 0);      // cho nha phim, moi lan nhan chi dem 1 lan
				delay_ms(20);

				press_count++;
				if (press_count > 2)
				{
					press_count = 1;
				}

				if (press_count == 1)
				{
					LEDS = 0x00;       // lan nhan thu 1: sang
				}
				else
				{
					LEDS = 0xFF;       // lan nhan thu 2: tat
				}
			}
		}
	}
}
