/*
 * i2c.h
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#ifndef I2C_H_
#define I2C_H_

#include "ossibeacon.h"

void i2c_set_readyFlag(void);
void i2c_clear_readyFlag(void);
uint8_t i2c_is_ready(void);

#endif /* I2C_H_ */
