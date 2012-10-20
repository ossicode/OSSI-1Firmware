/*
 * i2c.h
 *
 *  Created on: 2012. 5. 24.
 *      Author: donghee
 */

#ifndef I2C_H_
#define I2C_H_

#include  <msp430x16x.h>
#include "global.h"

void InitI2C(uint8_t slave_address);
void I2CWriteInit();
void I2CReadInit();

extern unsigned char I2CBuffer[3];
extern int PtrTransmit;
extern int PtrReceive;

#endif /* I2C_H_ */
