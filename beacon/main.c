/*
 *
                        .__           ____
   ____   ______  ______|__|         /_   |
  /  _ \ /  ___/ /  ___/|  |  ______  |   |
 (  <_> )\___ \  \___ \ |  | /_____/  |   |
  \____//____  >/____  >|__|          |___|
            \/      \/
  _
 | |
 | |__    ___   __ _   ___  ___   _ __
 | '_ \  / _ \ / _` | / __|/ _ \ | '_ \
 | |_) ||  __/| (_| || (__| (_) || | | |
 |_.__/  \___| \__,_| \___|\___/ |_| |_|

*  Created on: 2012. 6. 3.
*
*
*/

#include <msp430.h>				
#include "adf7020.h"
#include "morse.h"

#define PAON_PORT 2
#define PAON_PIN 7

char message[255] = {'o','s','s', 'i',' ','1',' ',' ',' ',' ',' ',' '};
int step = 10;

void configure_clock() {
	BCSCTL1 |= RSEL0 + RSEL1 + RSEL2;
	DCOCTL = DCO0 + DCO1 + DCO2; // MAX 8MHZ
	BCSCTL2 |= SELM_0 + DIVM_0;
}

void Poweramp_On() {
	IO_DIRECTION(PAON, OUTPUT);
	IO_SET(PAON, HIGH);
}

int main(void) {
	volatile unsigned int i;
	WDTCTL = WDTPW + WDTHOLD;		// Stop watchdog timer
	P1DIR |= BIT1;					// Set P1.0 to output direction
	// configure_clock();
	// default 1MHZ?

	Poweramp_On();

	ADF7020_1_Init();
	ADF7020_1_SendStart();

	while(1)
		adf7020_1_ook(1);

//	for(;;) {
//		  for(i=0; i < step-1;i++ ) {
//			  ascii(message[i]);
//		  }
//	}

}

