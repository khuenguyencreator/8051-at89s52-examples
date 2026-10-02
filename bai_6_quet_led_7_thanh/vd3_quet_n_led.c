// Bai 6 - Lap trinh 8051 quet LED 7 thanh
// Vi du 3: quet n LED 7 thanh Anode chung (module 7 so trong Proteus), hien thi bo dem tang dan
// AT89S52, thach anh 12MHz
// Doan a-g noi chung P0.0 - P0.6
// Chan chung cua LED thu k (tinh tu trai sang) cap nguon qua transistor PNP, chan B noi P2.k (muc 0 la bat)
#include <REGX52.H>

#define LED7     P0
#define DIGIT    P2
#define NUM_LED  7          // so LED 7 thanh, toi da 8 (P2.0 - P2.7)

// Ma LED 7 thanh Anode chung tu 0 den 9, muc 0 la sang
// LED khong co dau cham nen bit 7 (P0.7) de 0, neu LED co noi dp vao P0.7 thi dung 0xC0, 0xF9, 0xA4...
unsigned char code so[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

unsigned char chu_so[NUM_LED];     // chu_so[0] la hang don vi, chu_so[1] hang chuc, ...

// Tre khoang 1ms moi don vi voi thach anh 12MHz (Keil C51)
void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

// Tach so thanh tung chu so, tu hang don vi len
void Tach_So(unsigned long value)
{
	unsigned char i;
	for (i = 0; i < NUM_LED; i++)
	{
		chu_so[i] = value % 10;
		value = value / 10;
	}
}

// Quet 1 vong tat ca cac LED, moi LED sang 2ms
void Quet_Led(void)
{
	unsigned char k;
	for (k = 0; k < NUM_LED; k++)
	{
		LED7 = so[chu_so[NUM_LED - 1 - k]];   // LED ben trai nhat hien thi chu so cao nhat
		DIGIT &= ~(1 << k);                   // bat LED thu k
		delay_ms(2);
		DIGIT |= (1 << k);                    // tat LED thu k truoc khi doi ma
	}
}

void main(void)
{
	unsigned long dem = 0;
	unsigned char lan_quet;

	DIGIT = 0xFF;          // tat het cac LED

	while (1)
	{
		Tach_So(dem);
		for (lan_quet = 0; lan_quet < 10; lan_quet++)
		{
			Quet_Led();
		}
		dem++;
		if (dem > 9999999) dem = 0;    // 7 chu so
	}
}
