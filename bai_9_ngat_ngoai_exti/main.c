// Bai 9 - Lap trinh 8051 ngat ngoai EXTI dem xung
// AT89S52, thach anh 12MHz
// INT0 (P3.2): moi xung canh xuong tang bien dem, hien thi 0-9 tren LED 7 thanh
// INT1 (P3.3): xoa bien dem ve 0
#include <REGX52.H>

#define LED7 P2      // LED 7 thanh Anode chung: P2.0 = a ... P2.6 = g

sbit LED = P1^0;     // LED bao co xung

// Ma LED 7 thanh Anode chung tu 0 den 9
unsigned char code so[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

unsigned char count = 0;

// Ngat ngoai 0, vector so 0
void EX0_ISR(void) interrupt 0
{
	count++;
	if (count > 9)
	{
		count = 0;
	}
	LED = !LED;
}

// Ngat ngoai 1, vector so 2
void EX1_ISR(void) interrupt 2
{
	count = 0;
}

void main(void)
{
	LED = 1;

	IT0 = 1;    // INT0 kich hoat bang canh xuong
	IT1 = 1;    // INT1 kich hoat bang canh xuong
	EX0 = 1;    // cho phep ngat ngoai 0
	EX1 = 1;    // cho phep ngat ngoai 1
	PX0 = 1;    // INT0 uu tien cao hon INT1
	EA = 1;     // cho phep ngat toan cuc

	while (1)
	{
		LED7 = so[count];
	}
}
