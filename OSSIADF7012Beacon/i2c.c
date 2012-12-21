/*
 * i2c.c
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */
#include "i2c.h"

static volatile uint8_t i2c_rcv_flag;

volatile uint8_t RXData;


//void i2c_slave_init(void)
//{
//	// in slave mode, i2c can receive data even in LPM4 with ISR
//	i2c_rcv_flag = 0;
//	P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
//	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
//	UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
//	UCB0I2COA = 0x48;                         // Own Address is 048h
//	UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
//	IE2 |= UCB0RXIE;                          // Enable RX interrupt
//}

void i2c_set_readyFlag(void)
{
	i2c_rcv_flag = 1;
}

void i2c_clear_readyFlag(void)
{
	i2c_rcv_flag = 0;
}

uint8_t i2c_is_ready(void)
{
	if(i2c_rcv_flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t i2c_get_data()
{
	return RXData;
}

// USCI_B0 Data ISR
//#pragma vector = USCIAB0TX_VECTOR
//__interrupt void USCIAB0TX_ISR(void)
//{
//	// TODO: I2C and UART handle at the same time
//  RXData = UCB0RXBUF;                       // Get RX data
//  __bic_SR_register_on_exit(LPM3_bits);     // exits LPM3
//}
