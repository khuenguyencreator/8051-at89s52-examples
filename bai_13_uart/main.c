// Bai 13 - Lap trinh 8051 UART giao tiep voi may tinh
// AT89S52, thach anh 11.0592MHz, UART 9600 baud, 8 bit du lieu, 1 stop bit
// Gui '1' bat LED P1.0, gui '0' tat LED, moi ky tu nhan duoc deu duoc gui tra lai (echo)
#include <REGX52.H>

sbit LED = P1^0;

void UART_Init(void)
{
	SCON = 0x50;       // UART che do 1 (8 bit, baud thay doi duoc), REN = 1 cho phep nhan
	TMOD &= 0x0F;
	TMOD |= 0x20;      // Timer1 che do 2 (8 bit tu nap lai) tao toc do baud
	TH1 = 0xFD;        // 9600 baud voi thach anh 11.0592MHz
	TL1 = 0xFD;
	TR1 = 1;           // chay Timer1
	ES = 1;            // cho phep ngat UART
	EA = 1;            // cho phep ngat toan cuc
}

void UART_SendChar(unsigned char c)
{
	ES = 0;            // tam tat ngat UART de tu xu ly co TI
	SBUF = c;
	while (!TI);       // cho gui xong
	TI = 0;
	ES = 1;
}

void UART_SendString(char *s)
{
	while (*s)
	{
		UART_SendChar(*s++);
	}
}

// Ngat UART, vector so 4
void UART_ISR(void) interrupt 4
{
	unsigned char c;

	if (RI)
	{
		RI = 0;            // phai xoa co bang phan mem
		c = SBUF;
		if (c == '1')
		{
			LED = 0;       // LED noi len VCC: muc 0 la sang
		}
		else if (c == '0')
		{
			LED = 1;
		}
		SBUF = c;          // gui tra lai ky tu vua nhan
	}
	if (TI)
	{
		TI = 0;
	}
}

void main(void)
{
	LED = 1;
	UART_Init();
	UART_SendString("Hello Khuenguyencreator!\r\n");
	UART_SendString("Gui 1 de bat LED, 0 de tat LED\r\n");

	while (1)
	{
	}
}
