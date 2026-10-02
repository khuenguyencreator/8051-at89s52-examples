// Bai 4 - Lap trinh 8051 GPIO bat tat LED bang nut nhan
// Bai toan 2 - Cach 2: so sanh trang thai cu va moi de bat suon xuong cua nut nhan
// Khong bi treo khi giu nut, phu hop khi co nhieu nut nhan
// AT89S52, thach anh 12MHz
// LED noi P2.0 (muc 0 la sang), nut nhan noi P3.7
#include <REGX52.H>

sbit LED    = P2^0;
sbit BUTTON = P3^7;

bit button_old = 1;    // trang thai lan doc truoc, 1 = chua nhan

// Tre khoang 1ms moi don vi voi thach anh 12MHz (Keil C51)
void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

void main(void)
{
	bit button_new;

	LED = 1;

	while (1)
	{
		button_new = BUTTON;
		if ((button_old == 1) && (button_new == 0))   // suon xuong: vua nhan nut
		{
			LED = !LED;
		}
		button_old = button_new;   // cap nhat trang thai moi cua nut nhan
		delay_ms(20);              // doc nut moi 20ms de bo qua doi phim
	}
}
