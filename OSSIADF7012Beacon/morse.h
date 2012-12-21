/*
 * morse.h
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#ifndef MORSE_H_
#define MORSE_H_


#include "msp430x21x2.h"
#include "util.h"
#include "adf7012.h"
#include "system.h"

#define MAX_DATA_SIZE 59

void morse_set_sendFlag(void);
void morse_clear_sendFlag(void);

void morse_set_WPM(uint8_t wpm);

void morse_init(uint8_t wpm);
uint8_t morse_is_ready(void);
void morse_send_bytes(uint8_t * bytes);

#endif /* MORSE_H_ */
