// Bai 12 - Dieu che xung PWM tren 8051 (PWM mem bang ngat Timer0)
// AT89S52, thach anh 12MHz
// PWM xuat ra P1.0, tan so 100Hz, do phan giai 1%
// Nut P3.2 tang duty 10%, nut P3.3 giam duty 10%
#include <REGX52.H>

sbit PWM_OUT  = P1^0;
sbit BTN_UP   = P3^2;
sbit BTN_DOWN = P3^3;

unsigned char duty = 50;      // do rong xung 0 - 100 (%)
unsigned char pwm_count = 0;  // dem 0 - 99, moi buoc 100us

// Timer0 che do 2 (8 bit tu nap lai), tran sau moi 100us
void Timer0_Init(void)
{
	TMOD &= 0xF0;
	TMOD |= 0x02;      // Timer0 che do 2
	TH0 = 0x9C;        // 256 - 100 = 156 = 0x9C, tran sau 100 chu ky may = 100us
	TL0 = 0x9C;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

// Moi 100us vao ngat 1 lan, 100 lan = 1 chu ky PWM 10ms
void Timer0_ISR(void) interrupt 1
{
	pwm_count++;
	if (pwm_count >= 100)
	{
		pwm_count = 0;
	}
	PWM_OUT = (pwm_count < duty) ? 1 : 0;
}

void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

void main(void)
{
	Timer0_Init();

	while (1)
	{
		if (BTN_UP == 0)
		{
			delay_ms(20);                 // chong doi phim
			if (BTN_UP == 0)
			{
				if (duty <= 90) duty += 10;
				while (BTN_UP == 0);      // cho nha phim
			}
		}
		if (BTN_DOWN == 0)
		{
			delay_ms(20);
			if (BTN_DOWN == 0)
			{
				if (duty >= 10) duty -= 10;
				while (BTN_DOWN == 0);
			}
		}
	}
}
