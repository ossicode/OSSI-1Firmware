/*
 * gps.c
 *
 *  Created on: 2012. 12. 21.
 *      Author: OSSI
 */

#include "gps.h"

static volatile uint8_t gps_ready_flag = 0;

void gps_set_readyFlag(void)
{
	gps_ready_flag = 1;
}

void gps_clear_readyFlag(void)
{
	gps_ready_flag = 0;
}

uint8_t gps_is_ready(void)
{
	if(gps_ready_flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}


void gps_update_data(uint8_t raw_data)
{

}


