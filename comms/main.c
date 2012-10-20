#include <msp430.h>				
#include "adf7021n.h"
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
	configure_clock();

	adf7021n_init();
	// Ready to Send
	adf7021n_sendStart();
	// Ready to Receive
	// adf7021n_recvStart();
	_EINT();

	while(1) {
	    __delay_cycles(100000);
     }
}
