/*
 * main.c
 */
#include "ossibeacon.h"
#include "printf.h"
#include "adf7012.h"
#include "aclkuart.h"
#include "i2c.h"
#include "morse.h"
#include "gps.h"

void beacon_data_receive(void);
void beacon_data_processing(void);
void beacon_data_send(void);

void main(void)
{
	//first thing to do
	int_wdt_disable();
	clock_setup();

	//ports setup
	IO_setup();
	ext_wdt_setup();
	uart_setup_9600();
	ADF7012_setup();

	//module init
	uart_init();
	// TODO: check WDI timing
	ext_wdt_rst();


	// wait 1000 ms in the beginning for stabilizing 32.768kHz
	// TODO:implement ACLK clock stability check ->
	delay_ms(1);

	while(1)
	{
		// Enter LPM3, interrupts enabled
		__bis_SR_register(LPM3_bits + GIE);
		beacon_data_receive();
		beacon_data_processing();
		beacon_data_send();
	}
}


void beacon_data_receive(void) // uart related handler
{
	// gps
	if (uart_rx_ready())
	{
		uart_clear_rxFlag();
		// uart_get_byte() only works after uart ISR
		// TODO:check possibility of error!!!
		if(gps_update_data(uart_get_byte()))
		{
			// if all the gps data we want are received
			gps_set_readyFlag();
			return;
		}
	}
	else
	{
		return;
	}

	// i2c
	if (i2c_rx_ready())
	{
		i2c_clear_rxFlag();
	}
}

void beacon_data_processing(void)
{
	//gps
	// gps
	if (gps_is_ready())
	{
		gps_clear_readyFlag();
		// process gps data
		// make packet from i2c and gps data

		gps_make_packet();
		// ready to send morse code
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

void beacon_data_send(void) // timer0 related handler
{
	if(morse_is_ready())
	{
		// clear the flag
		morse_clear_sendFlag();
		morse_send_bytes(gps_get_stream());
	}
	else
	{
		return;
	}

}
