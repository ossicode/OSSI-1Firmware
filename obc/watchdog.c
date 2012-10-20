#include <msp430.h>
#include <global.h>


#define WATCHDOG_IN_PORT 1
#define WATCHDOG_IN_PIN 0


void Watchdog_External_Init() {
  WDTCTL = WDT_ADLY_1000;                    // WDT 1000ms, ACLK, interval timer
  IE1 |= WDTIE;                             // Enable WDT interrupt
  IO_DIRECTION(WATCHDOG_IN, OUTPUT);
  _BIS_SR(GIE);
}

// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  IO_SET(WATCHDOG_IN, TOGGLE);
}

