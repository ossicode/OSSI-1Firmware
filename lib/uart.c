#include  <msp430x16x.h>
#include "morse.h"
#include "global.h"

char uart_buffer[255];

uint8_t step = 0;
uint8_t sending = 0;

void initUart() {
	  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
	  ME1|= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
	  UCTL0 |= CHAR;                            // 8-bit character
	  UTCTL0 |= SSEL0;                          // UCLK = ACLK
	  UBR00 = 0x03;                             // 32k/9600 - 3.41
	  UBR10 = 0x00;                             //
	  UMCTL0 = 0x4A;                            // Modulation
	  UCTL0 &= ~SWRST;                          // Initialize USART state machine
	  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
}

void init_txrx_interrupt()
{
	  P1OUT = BIT2; //pull up
	  P1IE |= BIT2; // interrupt enable
	  P1IES |= BIT2; // interrupt hi/lo falling edge
	  //P1IES &= ~BIT2; // interrupt lo/hi edge
	  P1IFG &= ~BIT2; // P1.2 IFG cleared just in case

	  P5DIR |= BIT0; // TX

}
#pragma vector=USART0RX_VECTOR
__interrupt void usart0_rx (void)
{

  while (!(IFG1 & UTXIFG0));                // USART1 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF1 to TXBUF1
  uart_buffer[step] = RXBUF0;

  _EINT();

  step++;
  if (RXBUF0 == 13)   // CR
  {
	  //P6OUT ^= 0x01;
	  sending = 1;
  }
}
