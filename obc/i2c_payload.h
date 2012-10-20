/*
 * I2C_PAYLOAD.h
 *
 *  Created on: 2012. 5. 3.
 *      Author: donghee
 */

#ifndef I2C_PAYLOAD_H_
#define I2C_PAYLOAD_H_

#include  <msp430x16x.h>
#include "i2c.h"
#include "global.h"

void I2C_PAYLOAD_Write(uint8_t value);
void I2C_PAYLOAD_Init(uint8_t slave_address);
//uint8_t I2C_PAYLOAD_GetChannel(void);

#endif /* I2C_PAYLOAD_H_ */
