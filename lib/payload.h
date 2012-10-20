/*
 * morse.h
 *
 *  Created on: 2012. 4. 26.
 *      Author: donghee
 */

#ifndef PAYLOAD_H_
#define PAYLOAD_H_

#include  <msp430.h>
#include "global.h"

typedef enum {
	PING = 0,
	LIGHT_ON = 1,
	LIGHT_OFF = 2,
} PAYLOAD_CMD_ID;

typedef struct Command
{
	PAYLOAD_CMD_ID  id;
    uint8_t  length;
    uint8_t  data;
} Command;

typedef struct Command CMD;

#endif /* PAYLOAD_H_ */
