/*
 * setup2132.c
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#include "system.h"


void disable_int_wdt(void)
{
	WDTCTL = WDTPW + WDTHOLD;		// Stop watchdog timer
}

void config_clock(void)
{
	// default: MCLK = SMCLK = DCO ~ 1.2MHz
	// set DCO speed to calibrated 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
}

void init_IO(void)
{
	// Ext Watchdog Pin init
	P2DIR |= BIT2;
	IO_DIRECTION(LED,OUTPUT);
	IO_SET(LED,LOW);

}

void rst_ext_wdt(void)
{
	P2OUT &= ~BIT2; // default WDI = LOW
	delay_ms(1);
	P2OUT |= BIT2;
	delay_ms(1);
	P2OUT &= ~BIT2;
}


