/*
 * morse.c
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#include "morse.h"


//                      space .    ,    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F    G    H    I    J    K    L    M    N    O    P    Q    R    S    T    U    V    W    X    Y    Z
const uint8_t code[39] ={0x00,0x15,0x33,0x1F,0x0F,0x07,0x03,0x01,0x00,0x10,0x18,0x1C,0x1E,0x01,0x08,0x0A,0x04,0x00,0x02,0x06,0x00,0x00,0x07,0x05,0x04,0x03,0x02,0x07,0x06,0x0D,0x02,0x00,0x01,0x01,0x01,0x03,0x09,0x0B,0x0C};
const uint8_t size[39] ={4,6,6,5,5,5,5,5,5,5,5,5,5,2,4,4,3,1,4,3,4,2,4,3,4,2,2,3,4,4,3,3,1,3,4,3,4,4,4};

static volatile uint8_t morse_send_flag = 0;

static volatile uint16_t dot_length;
static volatile uint16_t total_dot_length;
static volatile uint16_t tick;

static volatile uint16_t bytes_size = 0;

void morse_timer_start(void)
{
	// sending morse code

	// default: set to 12 WPM, 1 dot = 100 ms //
	// 1 WPM = 50 dots per min
	// ex) 1 WPM, 1 dot = 1200ms
	TA0CCR0 = 33;						       // 33 -> 1.007 ms
	TA0CTL = TASSEL_1 + MC_1;                  // ACLK, upmode
	tick = 0;
	TA0CCTL0 |= CCIE;                           // TA0CCR0 interrupt enabled
}


void morse_set_WPM(uint8_t wpm)
{
	//min WPM = 1, 1 dot = 1200 ms
	//max WPM = 50, 1 dot = 24 ms
	//OSSI WPM = 12, 1 dot = 100 ms
	min_max(1,50,wpm);
	dot_length = (uint16_t)(1200 / wpm);
}

void morse_set_sendFlag(void)
{
	morse_send_flag = 1;
}

void morse_clear_sendFlag(void)
{
	morse_send_flag = 0;
}

uint8_t morse_is_ready(void)
{
	if(morse_send_flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint16_t morse_set_dataSizeFrom(uint8_t * bytes)
{
	volatile uint16_t bytes_size = 0;
	while(*bytes++)
	{
		bytes_size++;
	}

	return bytes_size;
}

void morse_init(uint8_t wpm)
{
	// TODO: add extra initialization process if needed
	morse_set_WPM(wpm);
	ADF7012_init();
	ADF7012_init_all_registers();
	ADF7012_enable();
	ADF7012_write_all_registers();
	if(ADF7012_lock())
	{
		morse_set_sendFlag();
		ADF7012_disable();
		return;
	}
	else
	{
		// TODO: do something when PLL is not locked!!
		// set vco bias and adj value with best guess for PLL
		morse_clear_sendFlag();
		ADF7012_disable();

	}
}

void morse_send_dots(uint8_t dots, uint8_t val)
{
	total_dot_length = 0;
	total_dot_length = dots * dot_length;
	ADF7012_OOK(val);
	IO_SET(LED,val);
	morse_timer_start();
}

void morse_send_bytes(uint8_t * bytes)
{
	// TODO: check initialization of static variables
	static volatile uint16_t bytes_cnt =0;
	static volatile uint16_t dot_cnt =0;
	static volatile uint8_t dot_sent = 0;
	volatile uint16_t MAX_DATA_SIZE;

	MAX_DATA_SIZE = morse_set_dataSizeFrom(bytes);

	// size[bytes[bytes_cnt]-65]: number of dots to send including pause dots
	if( bytes_cnt < MAX_DATA_SIZE)
	{

		volatile uint8_t conv_index;

		//if Capital Alphabet letter
		if(bytes[bytes_cnt] >= 'A' && bytes[bytes_cnt] <= 'Z')
		{
			conv_index = 52;
		}

		//if numbers
		if(bytes[bytes_cnt] >= '0' && bytes[bytes_cnt] <= '9')
		{
			conv_index = 45;
		}

		//if Capital Alphabet letter
		if(bytes[bytes_cnt] == ',' )
		{
			conv_index = 42;
		}

		//if Capital Alphabet letter
		if(bytes[bytes_cnt] == '.' )
		{
			conv_index = 45;
		}

		if(bytes[bytes_cnt] == ' ' )
		{
			dot_cnt = 0;
			bytes_cnt++;
			morse_send_dots(4,LOW);
			return;
		}

		volatile uint8_t total_dots = size[bytes[bytes_cnt]-conv_index];
		volatile uint8_t converted = code[bytes[bytes_cnt]-conv_index];

		if( dot_cnt < total_dots ) // count dots with pause added
		{
			if(dot_sent == 0)
			{
				dot_sent = 1;
				//check dash(1) or dot(0) from MSB
				if ((converted >> (total_dots - dot_cnt - 1)) & 0x01)
				{
					// 3 dots
					morse_send_dots(3,HIGH);
					return;
				}
				else
				{
					// 1 dot
					morse_send_dots(1,HIGH);
					return;
				}
			}

			if(dot_sent == 1)
			{
				dot_sent = 0;
				dot_cnt++;
				// if dots are sent
				// send 1 dot pause
				morse_send_dots(1,LOW);
				return;
			}
		}
		else // if a byte is sent
		{
				dot_cnt = 0;
				bytes_cnt++;
				morse_send_dots(2,LOW); // send 2 more dots so became total of 3 dots
				return;
		}
	}
	else // if all bytes are sent
	{
		dot_cnt = 0;
		bytes_cnt = 0;
		return;
	}

}

// Timer0_A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	// data processing
	// increase tick
	// compare tick to dotCnt
	// data processing
	tick++; // every 1.007 ms

	if (tick > total_dot_length)
	{
		tick = 0;
		// morse_set_sendFlag();
		morse_send_flag = 1; // replacing function call in ISR
		TA0CCTL0 &= ~CCIE; // disable timer interrupt
		// exit LPM3
		__bic_SR_register_on_exit(LPM3_bits);
	}


}
