/*
 *
                        .__           ____
   ____   ______  ______|__|         /_   |
  /  _ \ /  ___/ /  ___/|  |  ______  |   |
 (  <_> )\___ \  \___ \ |  | /_____/  |   |
  \____//____  >/____  >|__|          |___|
            \/      \/
                     
*  Created on: 2012. 6. 3.
*
*
*/

#include <msp430.h>				
#include "adf702x.h"
#include "morse.h"

char message[255] = {'o','s','s', 'i',' ','1',' ',' ',' ',' ',' ',' '};
int step = 10;

void configure_clock() {
	BCSCTL1 |= RSEL0 + RSEL1 + RSEL2;
	DCOCTL = DCO0 + DCO1 + DCO2;
	BCSCTL2 |= SELM_0 + DIVM_0;
}

int main(void) {
	volatile unsigned int i;
	WDTCTL = WDTPW + WDTHOLD;		// Stop watchdog timer
	P6DIR |= 0x01;					// Set P1.0 to output direction
//	configure_clock();

	adf7020_1_init();
	adf7020_1_sendStart();

	for(;;) {
		  for(i=0; i < step-1;i++ ) {
			  ascii(message[i]);
		  }

	}
}

