/*
 * util.c
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#include "util.h"

void delay_ms(uint16_t delay)
{
	volatile uint16_t i;
	for (i=0; i<delay;i++)
	{
		__delay_cycles(1000);
	}
}

void min_max(uint8_t min, uint8_t max, uint8_t value)
{
	if (value <= min)
	{
		value = min;
	}
	if (value > max)
	{
		value = max;
	}
}
