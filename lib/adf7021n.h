/*
 * adf7021n.h
 *
 *  Created on: 2012. 6. 18.
 *      Author: donghee
 */

#ifndef ADF7021N_H_
#define ADF7021N_H_

#include  <msp430x16x.h>
#include "global.h"

void adf7021n_init(void);
void adf7021n_sendStart(void);
void adf7021n_recvStart(void);

void adf7021n_tx(void);
void adf7021n_rx(void);

unsigned char adf7021n_get_mode(void);

#define IDLE 0
#define TX 1
#define RX 2

#define RF_MAX 64
#define PREAMBLE_BYTE           0xAA

#define VALID_PREAMBLE_BYTE_1   0x55
#define VALID_PREAMBLE_BYTE_2   0xAA

#define SYNC_WORD1 0xD3                // First byte of sync word
#define SYNC_WORD2 0x91                // Second byte of sync word

#define HEADER_SIZE     4       // 4 bytes header

#define ON  TRUE
#define OFF FALSE

#define adf702x_data       (adf702x_buf + 3)


#endif /* ADF7021N_H_ */
