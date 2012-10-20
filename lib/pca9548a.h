/*
 * PCA9548A.h
 *
 *  Created on: 2012. 5. 3.
 *      Author: donghee
 */

#ifndef PCA9548A_H_
#define PCA9548A_H_

#include  <msp430x16x.h>
#include "i2c.h"
#include "global.h"

void PCA9548A_SetChannel(uint8_t channel);
void PCA9548A_Init(uint8_t slave_address);
//uint8_t PCA9548A_GetChannel(void);

#endif /* PCA9548A_H_ */
