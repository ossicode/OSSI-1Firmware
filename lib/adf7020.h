/*
 * adf702x.h
 *
 *  Created on: 2012. 5. 5.
 *      Author: donghee
 */

#ifndef ADF7020_H_
#define ADF7020_H_

#include  <msp430x16x.h>
#include "global.h"

void ADF7020_1_Init(void);
void ADF7020_1_SendStart(void);
void adf7020_1_ook(uint8_t is_pa_on);

#endif /* ADF7020_H_ */
