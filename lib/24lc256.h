/*
 * 24lc256.h
 *
 *  Created on: 2012. 5. 3.
 *      Author: donghee
 */

#ifndef 24LC256_H_
#define 24LC256_H_

#include  <msp430x16x.h>
#include "i2c.h"
#include "global.h"

void EEPROM_ByteWrite(unsigned int Address, unsigned char Data);
void EEPROM_AckPolling();
unsigned char EEPROM_RandomRead(unsigned int Address);
unsigned char EEPROM_CurrentAddressRead();
void EEPROM_Init(uint8_t slave_address);


#endif /* 24LC256_H_ */
