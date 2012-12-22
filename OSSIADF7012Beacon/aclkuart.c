/*
 * aclkuart.c
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#include "aclkuart.h"
static volatile uint8_t uart_rx_flag = 0;
static volatile uint8_t rx_byte = '\0';

// Low-Frequency Baud Rate Mode
// read chapter 15.3.10 of msp430x2xxx user's guide for baud rate generation
void uart_setup_9600(void)
{
	P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD, No need to set IO direction
	UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
	UCA0BR0 = 0x03;                           // 3 = INT(3.41), 32.768kHz/9600 = 3.41
	UCA0BR1 = 0x00;                           //
	UCA0MCTL = UCBRS1 + UCBRS0;               // Modulation UCBRSx = 3 = round(0.41 * 8)
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void uart_setup_4800(void)
{
	P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD, No need to set IO direction
	UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
	UCA0BR0 = 0x06;                           // 6 = INT(6.83), 32.768kHz/4800 = 6.83
	UCA0BR1 = 0x00;                           //
	UCA0MCTL = UCBRS2 + UCBRS1 + UCBRS0;      // Modulation UCBRSx = 7 = round(0.83 * 8)
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void uart_init(void)
{
	IE2 |= UCA0RXIE;                          // default: Enable USCI_A0 RX interrupt
	IE2 &= ~UCA0TXIE;						  // default: Disable USCI_A0 TX interrupt
}

void i2c_set_rxFlag(void)
{

}

void i2c_clear_rxFlag(void)
{

}

uint8_t i2c_rx_ready(void)
{
	return 0;
}

void uart_set_rxFlag(void)
{
	uart_rx_flag = 1;
}

void uart_clear_rxFlag(void)
{
	uart_rx_flag = 0;
}

uint8_t uart_rx_ready(void)
{
	if(uart_rx_flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t uart_get_byte(void)
{
	// return only you have available data from uart, NOT i2c

	return rx_byte;
}


/**
 * Sends a single byte out through UART
 **/
void uart_send_byte(unsigned char byte)
{

	while (!(IFG2&UCA0TXIFG));			// USCI_A0 TX buffer ready? //TODO: change this to avoid infinite loop!!!!!
	UCA0TXBUF = byte;					// TX -> RXed character

}

void puts(char *s) {
	char c;

	// Loops through each character in string 's'
	while (c = *s++) {
		uart_send_byte(c);
	}
}
/**
 * puts() is used by printf() to display or send a character. This function
 *     determines where printf prints to. For this case it sends a character
 *     out over UART.
 **/
void putc(unsigned b) {
	uart_send_byte(b);
}


// USCI A0/B0 Transmit ISR
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
	IE2 &= ~UCA0TXIE;						// Disable USCI_A0 TX interrupt
}

// USCI A0/B0 Receive ISR
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	// TODO: I2C and UART handle at the same time
	rx_byte = UCA0RXBUF;					// Get the received character

	// gps (uart)
	uart_set_rxFlag();
	__bic_SR_register_on_exit(LPM3_bits);


}

