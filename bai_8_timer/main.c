// Bai 8 - Lap trinh 8051 Timer
// AT89S52, thach anh 12MHz => 1 chu ky may = 1us
// LED1 (P1.0): nhay 500ms bang ngat Timer0
// LED2 (P1.1): nhay 1000ms bang ham delay dung Timer1 (hoi co TF1)
#include <REGX52.H>

sbit LED1 = P1^0;
sbit LED2 = P1^1;

unsigned int ms_count = 0;

// Timer0 che do 1 (16 bit), tran sau 1000us = 1ms
void Timer0_Init(void)
{
	TMOD &= 0xF0;      // xoa 4 bit cau hinh Timer0
	TMOD |= 0x01;      // Timer0 che do 1
	TH0 = 0xFC;        // 65536 - 1000 = 64536 = 0xFC18
	TL0 = 0x18;
	ET0 = 1;           // cho phep ngat Timer0
	EA = 1;            // cho phep ngat toan cuc
	TR0 = 1;           // bat dau dem
}

// Ngat Timer0, vector so 1
void Timer0_ISR(void) interrupt 1
{
	TH0 = 0xFC;        // che do 1 khong tu nap lai, phai nap lai bang tay
	TL0 = 0x18;
	ms_count++;
	if (ms_count >= 500)
	{
		ms_count = 0;
		LED1 = !LED1;
	}
}

// Tre chinh xac 1ms moi don vi bang Timer1 che do 1
void delay_ms(unsigned int ms)
{
	TMOD &= 0x0F;      // xoa 4 bit cau hinh Timer1
	TMOD |= 0x10;      // Timer1 che do 1
	while (ms--)
	{
		TH1 = 0xFC;
		TL1 = 0x18;
		TF1 = 0;
		TR1 = 1;
		while (!TF1);  // cho tran
		TR1 = 0;
	}
}

void main(void)
{
	LED1 = 1;
	LED2 = 1;
	Timer0_Init();

	while (1)
	{
		LED2 = !LED2;
		delay_ms(1000);
	}
}
