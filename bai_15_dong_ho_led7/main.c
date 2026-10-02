// Bai 15 - Du an dong ho LED 7 thanh 4 so voi 8051
// AT89S52, thach anh 12MHz
// LED 7 thanh 4 so Anode chung: doan a-g, dp noi P2.0 - P2.7, 4 chan chung qua transistor PNP noi P1.0 - P1.3 (muc 0 la bat)
// Hien thi gio:phut, dau cham giua nhay theo giay. Nut P3.2 tang gio, nut P3.3 tang phut
#include <REGX52.H>

#define SEG     P2          // doan a-g, dp
#define DIGIT   P1          // P1.0 - P1.3 chon so

sbit BTN_HOUR   = P3^2;
sbit BTN_MINUTE = P3^3;

// Ma LED 7 thanh Anode chung tu 0 den 9 (dp tat)
unsigned char code so[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

unsigned char hour = 12, minute = 0, second = 0;
unsigned int  ms_count = 0;
unsigned char led_buf[4];      // ma hien thi cua 4 so
unsigned char digit_index = 0;

// Timer0 che do 1, ngat moi 1ms
void Timer0_Init(void)
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = 0xFC;        // 65536 - 1000
	TL0 = 0x18;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

// Moi 1ms: quet 1 so LED va dem thoi gian
void Timer0_ISR(void) interrupt 1
{
	TH0 = 0xFC;
	TL0 = 0x18;

	// 1. Quet LED: tat het cac so, doi ma doan, bat so tiep theo
	DIGIT |= 0x0F;
	SEG = led_buf[digit_index];
	DIGIT &= ~(1 << digit_index);
	digit_index++;
	if (digit_index >= 4) digit_index = 0;

	// 2. Dem thoi gian
	ms_count++;
	if (ms_count >= 1000)
	{
		ms_count = 0;
		second++;
		if (second >= 60)
		{
			second = 0;
			minute++;
			if (minute >= 60)
			{
				minute = 0;
				hour++;
				if (hour >= 24) hour = 0;
			}
		}
	}
}

void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

// Cap nhat bo dem hien thi tu gio, phut
void Update_Display(void)
{
	led_buf[0] = so[hour / 10];
	led_buf[1] = so[hour % 10];
	if (ms_count < 500)
	{
		led_buf[1] &= 0x7F;    // bat dau cham (bit 7 = dp) trong nua giay dau
	}
	led_buf[2] = so[minute / 10];
	led_buf[3] = so[minute % 10];
}

void main(void)
{
	DIGIT |= 0x0F;             // tat het cac so
	Timer0_Init();

	while (1)
	{
		Update_Display();

		if (BTN_HOUR == 0)
		{
			delay_ms(20);
			if (BTN_HOUR == 0)
			{
				hour++;
				if (hour >= 24) hour = 0;
				while (BTN_HOUR == 0) Update_Display();
			}
		}
		if (BTN_MINUTE == 0)
		{
			delay_ms(20);
			if (BTN_MINUTE == 0)
			{
				minute++;
				if (minute >= 60) minute = 0;
				second = 0;
				while (BTN_MINUTE == 0) Update_Display();
			}
		}
	}
}
