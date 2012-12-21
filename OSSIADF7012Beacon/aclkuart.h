/*
 * aclkuart.h
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#ifndef ACLKUART_H_
#define ACLKUART_H_

#include "msp430x21x2.h"
#include "util.h"
#include "gps.h"




void UART_setup_4800(void);
void UART_setup_9600(void);
void UART_init(void);
void putc(unsigned);
void puts(char *);

#define MAXCOMMAS 14   //Max commas to store positions of (14 for GGA sentence)
#define MAXGPS 82              //Max NMEA string size
#define MAXLAT 9               //Max latitude size in bytes
#define MAXLONG 10             //Max longitude size in bytes
#define MAXTIME 10             //Max time size in bytes
#define MAXSATS 2              //Max sats size in bytes
#define MAXALTI 8              //Max altitude size in bytes

typedef struct
{
       uint8_t lat[MAXLAT+1];
       uint8_t lng[MAXLONG+1];
       uint8_t time[MAXTIME+1];
       uint8_t sats[MAXSATS+1];
       uint8_t alti[MAXALTI+1];
       uint8_t ns;
       uint8_t ew;
       uint8_t valid;
} Gps;

#endif /* ACLKUART_H_ */
