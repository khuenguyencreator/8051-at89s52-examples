// Bai 4 - Lap trinh 8051 GPIO bat tat LED bang nut nhan
// Vi du 1: LED sang khi giu nut, tat khi nha nut
// AT89S52, thach anh 12MHz
// LED noi P2.0 kieu hut dong (muc 0 la sang), nut nhan noi P3.7 xuong GND
#include <REGX52.H>

sbit LED    = P2^0;
sbit BUTTON = P3^7;

void main(void)
{
	BUTTON = 1;        // ghi 1 truoc khi dung chan lam ngo vao

	while (1)
	{
		LED = BUTTON;  // nhan nut: P3.7 = 0 => P2.0 = 0 => LED sang
	}
}
