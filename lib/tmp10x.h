/*
 * tmp10x.h
 *
 *  Created on: 2012. 5. 3.
 *      Author: donghee
 */

#ifndef TMP10x_H_
#define TMP10x_H_

#include  <msp430x16x.h>
#include "i2c.h"
#include "global.h"

uint16_t TMP10X_Read(void);
void TMP10x_Init(uint8_t slave_address);


#endif /* TMP10x_H_ */
