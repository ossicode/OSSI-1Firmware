/*
 * morse.h
 *
 *  Created on: 2012. 4. 26.
 *      Author: donghee
 */

#ifndef MORSE_H_
#define MORSE_H_

#include  <msp430x16x.h>
#include "global.h"

#define LED_PORT           1
#define LED_PIN            1

//#define LED_ON()           IO_SET(LED,HIGH)
//#define LED_OFF()           IO_SET(LED,LOW)

#define LED_ON()
#define LED_OFF()

void ascii(char c);

#endif /* MORSE_H_ */
