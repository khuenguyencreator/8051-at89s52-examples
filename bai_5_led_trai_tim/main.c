// Bai 5 - Lap trinh 8051 LED trai tim voi cac hieu ung
// AT89S52, thach anh 12MHz, mo phong Proteus
// 32 LED noi P0.0 - P3.7 theo chieu kim dong ho, kieu hut dong:
// anode noi VCC (qua dien tro), cathode noi chan vi dieu khien => muc 0 la sang, muc 1 la tat
#include <REGX52.H>

#define LED_ON   0x00
#define LED_OFF  0xFF
#define STEP_MS  100      // thoi gian moi buoc hieu ung

// Bang ma sang dan tu bit 0 len bit 7 (muc 0 la sang)
unsigned char code sang_thuan[8] = {0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00};
// Bang ma sang dan tu bit 7 xuong bit 0
unsigned char code sang_nguoc[8] = {0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00};
// Bang ma tat dan tu bit 0 len bit 7
unsigned char code tat_thuan[8]  = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
// Bang ma tat dan tu bit 7 xuong bit 0
unsigned char code tat_nguoc[8]  = {0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};

// Tre khoang 1ms moi don vi voi thach anh 12MHz (Keil C51)
void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

void Set_All(unsigned char value)
{
	P0 = value;
	P1 = value;
	P2 = value;
	P3 = value;
}

// Ghi gia tri ra 1 port theo so thu tu 0 - 3
void Set_Port(unsigned char port, unsigned char value)
{
	switch (port)
	{
		case 0: P0 = value; break;
		case 1: P1 = value; break;
		case 2: P2 = value; break;
		case 3: P3 = value; break;
	}
}

// Hieu ung 1: 32 LED cung nhap nhay
void Nhap_Nhay(void)
{
	Set_All(LED_ON);
	delay_ms(300);
	Set_All(LED_OFF);
	delay_ms(300);
}

// Hieu ung 2: sang dan theo chieu kim dong ho, P0.0 -> P3.7
void Sang_Dan_Thuan(void)
{
	unsigned char port, i;

	Set_All(LED_OFF);
	for (port = 0; port < 4; port++)
	{
		for (i = 0; i < 8; i++)
		{
			Set_Port(port, sang_thuan[i]);
			delay_ms(STEP_MS);
		}
	}
}

// Hieu ung 3: sang dan nguoc chieu kim dong ho, P3.7 -> P0.0
void Sang_Dan_Nguoc(void)
{
	unsigned char port, i;

	Set_All(LED_OFF);
	for (port = 4; port > 0; port--)
	{
		for (i = 0; i < 8; i++)
		{
			Set_Port(port - 1, sang_nguoc[i]);
			delay_ms(STEP_MS);
		}
	}
}

// Hieu ung 4: tat dan theo chieu kim dong ho
void Tat_Dan_Thuan(void)
{
	unsigned char port, i;

	Set_All(LED_ON);
	for (port = 0; port < 4; port++)
	{
		for (i = 0; i < 8; i++)
		{
			Set_Port(port, tat_thuan[i]);
			delay_ms(STEP_MS);
		}
	}
}

// Hieu ung 5: tat dan nguoc chieu kim dong ho
void Tat_Dan_Nguoc(void)
{
	unsigned char port, i;

	Set_All(LED_ON);
	for (port = 4; port > 0; port--)
	{
		for (i = 0; i < 8; i++)
		{
			Set_Port(port - 1, tat_nguoc[i]);
			delay_ms(STEP_MS);
		}
	}
}

// Hieu ung 6: so le nho giot, 1 LED chay tu bit 0 toi cuoi roi dung lai, cac LED da roi xuong giu nguyen
// Lam dong thoi tren ca 4 port
void So_Le_Nho_Giot(void)
{
	unsigned char i, j;
	unsigned char da_sang = 0x00;      // cac LED da roi xuong (bit 1 = sang)

	for (i = 8; i > 0; i--)
	{
		for (j = 0; j < i; j++)
		{
			Set_All(~((0x01 << j) | da_sang));   // dao bit vi muc 0 moi la sang
			delay_ms(STEP_MS);
		}
		da_sang |= 0x01 << (i - 1);    // giot vua roi nam lai o vi tri cuoi
	}
}

void main(void)
{
	unsigned char n;

	while (1)
	{
		for (n = 0; n < 5; n++) Nhap_Nhay();
		for (n = 0; n < 5; n++) Sang_Dan_Thuan();
		for (n = 0; n < 5; n++) Sang_Dan_Nguoc();
		for (n = 0; n < 5; n++) Tat_Dan_Thuan();
		for (n = 0; n < 5; n++) Tat_Dan_Nguoc();
		for (n = 0; n < 5; n++) So_Le_Nho_Giot();
	}
}
