#include "morse.h"
#include "adf7020.h"

//uint8_t done = 0b10000000;
uint8_t done = 0x80;
uint8_t output_pin;

void delay(void)
{
    volatile unsigned int i;
//    i = 10000;                              // Delay
    i = 5000;                              // Delay
    do (i--);
    while (i != 0);
}

void mark(uint8_t t)
{
	volatile unsigned int i;
	adf7020_1_ook(1);
//	P1OUT &= ~BIT1;
	LED_ON();
	for(i = 0; i < t; i++)
	{                         // Turn On
		delay();
	}
	adf7020_1_ook(0);
	LED_OFF();                          // Turn Off
	delay();
}

void space(uint8_t t)
{
	volatile unsigned int i;
	adf7020_1_ook(0);
	LED_OFF();

	for(i = 0; i < t; i++)
	{
		delay();
	}
}

void morse(uint8_t m)
{
	while (m != done) {
   		mark(m & done ? 3 : 1);
   		space(1);                                                                                                                                             		m = m << 1;
 	}
	space(3);
}


void ascii(char c) {
	P1DIR |= BIT1;
	uint8_t number_code[] = {
// 		0b11111100,	0b01111100, 0b00111100, 0b00011100, 0b00001100, 0b00000100,
// 		0b10000100, 0b11000100, 0b11100100, 0b11110100 // 0-9
		0xfc, 0x7c, 0x3c, 0x1c, 0xc, 0x4, 0x84, 0xc4, 0xe4, 0xf4
	};

	uint8_t alphabet_code[] = {
//		0b01100000, 0b10001000, 0b10101000, 0b10010000, 0b01000000, 0b00101000,
//		0b11010000, 0b00001000, 0b00100000, 0b01111000, 0b10110000, 0b01001000,
//		0b11100000, 0b10100000, 0b11110000, 0b01101000, 0b11011000, 0b01010000,
//		0b00010000, 0b11000000, 0b00110000, 0b00011000, 0b01110000, 0b10011000,
//		0b10111000, 0b11001000 // A-Z
		0x60, 0x88, 0xa8, 0x90, 0x40, 0x28, 0xd0, 0x8, 0x20, 0x78, 0xb0, 0x48, 0xe0, 0xa0, 0xf0, 0x68, 0xd8, 0x50, 0x10, 0xc0, 0x30, 0x18, 0x70, 0x98, 0xb8, 0xc8
	};

	if (c >= '0' && c <= '9') {
		morse(number_code[c- '0']);
	} else if (c >= 'a' && c <= 'z') {
		morse(alphabet_code[c - 'a']);
	} else if (c == ' ') {
		space(7);
	} else {
		morse(done);
	}
}
