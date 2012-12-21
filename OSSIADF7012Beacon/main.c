/*
 * main.c
 */
#include "msp430x21x2.h"
#include "system.h"
#include "util.h"
#include "printf.h"
#include "adf7012.h"
#include "aclkuart.h"
#include "i2c.h"
#include "morse.h"
#include "gps.h"


uint8_t i2c_data[10] = {'C','Q','C','Q','C','Q','1',',','9','0'};
uint8_t beacon_data[1]={0};
extern uint8_t gps_array_data[59];

void beacon_msg_send(void);
void beacon_data_receive(void);
void i2c_data_check(void);

void main(void)
{
	//first thing to do
	disable_int_wdt();
	config_clock();

	//ports setup
	init_IO();
	UART_setup_9600();
	ADF7012_setup();

	//module init
	UART_init();
	// TODO: check WDI timing
	rst_ext_wdt();


	// wait 1000 ms in the beginning for stabilizing 32.768kHz
	// TODO:implement ACLK clock stability check ->
	delay_ms(1);

	//morse_init(12);

	while(1)
	{
		__bis_SR_register(LPM3_bits + GIE);
		beacon_data_receive();
		beacon_msg_send();

		// Enter LPM3, interrupts enabled


	}
}

//void i2c_data_check(void)
//{
//	if(i2c_is_data_received())
//	{
//		// clear the flag
//		i2c_set_recieved();
//		// init flag and init RF transmitter
//		morse_init();
//	}
//	else
//	{
//		return;
//	}
//}

void beacon_data_receive(void) // uart related handler
{
	// gps
	if (gps_is_ready())
	{
		gps_clear_readyFlag();
		morse_init(20);
	}
	else
	{
		return;
	}


	// i2c
	if (i2c_is_ready())
	{
		i2c_clear_readyFlag();
		morse_init(12);

	}
}

void beacon_msg_send(void) // timer0 related handler
{
	if(morse_is_ready())
	{
		// clear the flag
		morse_clear_sendFlag();
		morse_send_bytes(gps_array_data);
	}
	else
	{
		return;
	}

}
