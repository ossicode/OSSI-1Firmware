/*
 * setup2132.h
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#ifndef SETUP2132_H_
#define SETUP2132_H_

#include "msp430x21x2.h"
#include "util.h"

// System IO Define
#define LED_PORT 3
#define LED_PIN 0


void disable_int_wdt(void);
void config_clock(void);
void init_IO(void);
void rst_ext_wdt(void);

#endif /* SETUP2132_H_ */
