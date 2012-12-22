/*
 * setup2132.c
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#include "system.h"


void int_wdt_disable(void)
{
	WDTCTL = WDTPW + WDTHOLD;		// Stop watchdog timer
}

void clock_setup(void)
{
	// default: MCLK = SMCLK = DCO ~ 1.2MHz
	// set DCO speed to calibrated 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
}

void IO_setup(void)
{
	// general IO init
	// TODO: set unused pins!!!

	IO_DIRECTION(LED,OUTPUT);
	IO_SET(LED,LOW);

}

void ext_wdt_setup(void)
{
	IO_DIRECTION(EXTWDT,OUTPUT);
}

void ext_wdt_rst(void)
{
	//LOW-HIGH-LOW
	IO_SET(EXTWDT,LOW);
	delay_ms(1);
	IO_SET(EXTWDT,HIGH);
	delay_ms(1);
	IO_SET(EXTWDT,LOW);
}


