#include "hardware.h"

void configure_clock() {
	BCSCTL1 |= RSEL0 + RSEL1 + RSEL2;
	DCOCTL = DCO0 + DCO1 + DCO2;
	BCSCTL2 |= SELM_0 + DIVM_0;
}

void stop_watchdog() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
}

void delay_one_sec() { // it depense on clock speed
	__delay_cycles(500000);
}