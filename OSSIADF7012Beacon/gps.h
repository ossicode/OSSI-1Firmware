/*
 * gps.h
 *
 *  Created on: 2012. 12. 21.
 *      Author: OSSI
 */

#ifndef GPS_H_
#define GPS_H_

#include "msp430x21x2.h"
#include "util.h"
#include "aclkuart.h"

void gps_set_readyFlag(void);
void gps_clear_readyFlag(void);
uint8_t gps_is_ready(void);

void gps_update_data(uint8_t raw_data);


#endif /* GPS_H_ */
