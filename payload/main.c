/*
 *
                        .__           ____
   ____   ______  ______|__|         /_   |
  /  _ \ /  ___/ /  ___/|  |  ______  |   |
 (  <_> )\___ \  \___ \ |  | /_____/  |   |
  \____//____  >/____  >|__|          |___|
            \/      \/
 
 ______           _                 _ 
 | ___ \         | |               | |
 | |_/ /_ _ _   _| | ___   __ _  __| |
 |  __/ _` | | | | |/ _ \ / _` |/ _` |
 | | | (_| | |_| | | (_) | (_| | (_| |
 \_|  \__,_|\__, |_|\___/ \__,_|\__,_|
             __/ |                    
            |___/                     

*  MSP430F2012
*
*  Created on: 2012. 6. 3.
*
*
*/
#include "msp430x20x2.h"
#include "payload.h"

#define LED_PORT 1
#define LED_PIN 1

#define PWM_PORT 1
#define PWM_PIN 5

int PtrReceive=0;

uint8_t cmd;
uint8_t length;
uint8_t data;

//int I2C_Slave_Init(uint16_t address)
//{
//	  P3SEL |= 0x0A;                            // Select I2C pins
//	  U0CTL |= I2C + SYNC;                      // Recommended init procedure
//	  U0CTL &= ~I2CEN;                          // Recommended init procedure
//	  I2CTCTL |= I2CSSEL1;                      // SMCLK
//	  I2COA = 0x0048;                           // Own Address is 048h
//	  I2CIE = RXRDYIE;                          // Enable RXRDYIFG interrupt
//	  U0CTL |= I2CEN;                           // Enable I2C
//}

void LED_Init(void) {
	  P1DIR |= BIT2;                            // P1.2 output
	  P1SEL |= BIT2;                            // P1.2 TA1 options
}

void LED_On(void) {
 	CCR0 = 500;                             // PWM Period
    CCTL1 = OUTMOD_7;                         // CCR1 reset/set
	CCR1 = 150;                              // CCR1 PWM duty cycle
	TACTL = TASSEL_2 + MC_1;                  // SMCLK, up mode
}

void LED_Off(void) {
	TACTL = ~TAIE;
}

void LED_Blink(uint32_t on_time, uint32_t off_time)
{
	LED_On();

	__delay_cycles(150000);

	LED_Off();

	__delay_cycles(150000);
}

int main(void) {
	volatile unsigned int i;
	volatile unsigned int x;
	volatile unsigned int z;
	WDTCTL = WDTPW + WDTHOLD;		// Stop watchdog timer
	_EINT();

	LED_Init();

	while(1)
		LED_Blink(150000, 150000);
}

void parse_command(uint8_t i2c_data)
{
	  if (PtrReceive == 0) {
		  cmd = i2c_data;
	  }
	  if (PtrReceive == 1) {
		  length = i2c_data;
	  }
	  if (PtrReceive == 2)
	  {
		  data = i2c_data; //TODO: It just one byte.
	  }

	  if (PtrReceive == length+1)
	  {
	      if (data == 0) {
	    	  LED_OFF();
	      }
	      if (data == 1) {
	    	  LED_ON();
	      }
	      PtrReceive = -1;
	  }

	  PtrReceive = PtrReceive+1;
}


//// Common ISR for I2C Module
//#pragma vector=USART0TX_VECTOR
//__interrupt void I2C_ISR(void)
//{
// switch( I2CIV )
// {
//   case  2: break;                          // Arbitration lost
//   case  4: break;                          // No Acknowledge
//   case  6: break;                          // Own Address
//   case  8: break;                          // Register Access Ready
//   case 10:                                 // Receive Ready
//	  parse_command(I2CDRB);
//	  break;
////
////      RXData = I2CDRB;                      // RX data
////      if (RXData > 0) {
////    	  LED_ON();
////      }else {
////    	  LED_OFF();
////      }
////     _BIC_SR_IRQ(CPUOFF);                   // Clear LPM0
//     break;
//   case 12:  break;                         // Transmit Ready
//   case 14: break;                          // General Call
//   case 16: break;                          // Start Condition
// }
//}
