// Bai 4 - Lap trinh 8051 GPIO bat tat LED bang nut nhan
// Vi du 2: nhan BT1 thi 8 LED D1-D8 sang, nhan BT2 thi 8 LED tat
// AT89S52, thach anh 12MHz
// 8 LED noi P2.0 - P2.7 (muc 0 la sang), BT1 noi P1.0, BT2 noi P1.7
#include <REGX52.H>

#define LEDS P2

sbit BT1 = P1^0;
sbit BT2 = P1^7;

void main(void)
{
	LEDS = 0xFF;           // ban dau tat het 8 LED

	while (1)
	{
		if (BT1 == 0)
		{
			LEDS = 0x00;   // sang het 8 LED
		}
		if (BT2 == 0)
		{
			LEDS = 0xFF;   // tat het 8 LED
		}
	}
}
