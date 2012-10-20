#include <msp430.h>
#include "morse.h"

char message[255] = {'1','2','3','l','o',' ','o','s','s','i',' ', '1'};

int main(void) {
	volatile unsigned int i;
    volatile unsigned char x;
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

    EEPROM_Init(0x50);

	for(i=0; i < 3;i++ ) {
		  ascii(message[i]);
		  EEPROM_ByteWrite(i, message[i]);
		  EEPROM_AckPolling();
	}

	for(i=0; i < 3;i++ ) {
		  x=EEPROM_RandomRead(i);
		  ascii(x);
	}
 }

