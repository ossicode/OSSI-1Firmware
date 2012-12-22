/*
 * setup2132.h
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#ifndef SETUP2132_H_
#define SETUP2132_H_

#include "msp430x21x2.h"
#include "ossitypes.h"
#include "util.h"

// System IO Define
#define LED_PORT 3
#define LED_PIN 0

#define EXTWDT_PORT 2
#define EXTWDT_PIN 2

void int_wdt_disable(void);
void clock_setup(void);
void IO_setup(void);
void ext_wdt_setup(void);

void ext_wdt_rst(void);

#endif /* SETUP2132_H_ */
