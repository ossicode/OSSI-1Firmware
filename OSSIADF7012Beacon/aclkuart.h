/*
 * aclkuart.h
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#ifndef ACLKUART_H_
#define ACLKUART_H_

#include "ossibeacon.h"

void uart_setup_4800(void);
void uart_setup_9600(void);
void uart_init(void);

void uart_set_rxFlag(void);
void uart_clear_rxFlag(void);
uint8_t uart_rx_ready(void);

void i2c_set_rxFlag(void);
void i2c_clear_rxFlag(void);
uint8_t i2c_rx_ready(void);

uint8_t uart_get_byte(void);

void putc(unsigned);
void puts(char *);




#endif /* ACLKUART_H_ */
