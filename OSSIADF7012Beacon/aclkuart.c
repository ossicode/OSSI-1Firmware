/*
 * aclkuart.c
 *
 *  Created on: 2012. 12. 19.
 *      Author: OSSI
 */

#include "aclkuart.h"

uint8_t uart_byte = '\0';
uint8_t rawgps[82];
uint8_t gpsgga[82];
uint8_t gpsindex = 0;
uint8_t gpsdone = 0;
Gps gpsdata = {"3518.5230\0", "12039.6090\0", "123456.789\0", "00\0", "000000.0\0", 'N', 'W', '0'};

uint8_t gps_array_data[59]={0};

void gps_parse_nmea(void)
{
	IE2 &= ~UCA0RXIE; //Disable receive interrupt
	uint8_t commas[MAXCOMMAS];
	uint8_t i = 0, n;
	for(n=0;n<MAXCOMMAS;n++) //Find the positions of all commas in the NMEA sentence, put positions in commas[]
	{
		   for(;rawgps[i]!=','||i>MAXGPS;i++); //Find next comma; continue stepping through  the array until we find 0x2C (,)
		   commas[n] = i; //Store the index in commas[] array
		   i++;
	}
	if(rawgps[commas[5]+1] != '0') //Make sure we have GPS fix; 0 = invalid
	{
		   for(i=commas[1]+1;i<commas[2];i++)
		   {
				   gpsdata.lat[i-(commas[1]+1)] = rawgps[i]; //Load latitude into lat[] array from stored NMEA string
		   }
		   gpsdata.lat[i-(commas[1])-1] = '\0';
		   gpsdata.ns = rawgps[commas[2]+1];

		   for(i=commas[3]+1;i<commas[4];i++)
		   {
				   gpsdata.lng[i-(commas[3]+1)] = rawgps[i]; //Load longitude into lng[] array from stored NMEA string
		   }
		   gpsdata.lng[i-(commas[3])-1] = '\0';
		   gpsdata.ew = rawgps[commas[4]+1];

		   for(i=commas[6]+1;i<commas[7];i++)
		   {
				   gpsdata.sats[i-(commas[6]+1)] = rawgps[i]; //Load sats into sats[] array from stored NMEA string
		   }
		   gpsdata.sats[i-(commas[6])-1] = '\0';

		   for(i=commas[8]+1;i<commas[9];i++)
		   {
				   gpsdata.alti[i-(commas[8]+1)] = rawgps[i]; //Load alt into alti[] array from stored NMEA string
		   }
		   gpsdata.alti[i-(commas[8])-1] = '\0';

		   for(i=commas[0]+1;i<commas[1];i++)
		   {
				   gpsdata.time[i-(commas[0]+1)] = rawgps[i]; //Load time into time[] array from stored NMEA string
		   }
		   gpsdata.time[i-(commas[0])-1] = '\0';
		   gpsdata.valid = rawgps[commas[5]+1];
	}
	else //Else update the timestamp, but retain old GPS data
	{
		   for(i=commas[0]+1;i<commas[1];i++)
		   {
				   gpsdata.time[i-(commas[0]+1)] = rawgps[i];
		   }
		   gpsdata.time[i-(commas[0])-1] = '\0';

		   for(i=commas[6]+1;i<commas[7];i++)
		   {
				   gpsdata.sats[i-(commas[6]+1)] = rawgps[i]; //Load sats into sats[] array from stored NMEA string
		   }
		   gpsdata.sats[i-(commas[6])-1] = '\0';
		   gpsdata.valid = '0';
	}
	IE2 |= UCA0RXIE; //Enable receive interrupt
}

void gps_convert_stream(void)
{
	volatile uint8_t i;
	volatile uint8_t step = 0;
	gps_array_data[step] ='C';
	step++;
	gps_array_data[step] ='Q';
	step++;
	gps_array_data[step] ='C';
	step++;
	gps_array_data[step] ='Q';
	step++;
	gps_array_data[step] ='C';
	step++;
	gps_array_data[step] ='Q';
	step++;
	gps_array_data[step] =',';
	step++;

	for (i = 0; i < 10 ; i++) {
		gps_array_data[step] = gpsdata.lat[i];
		step++;
	}

	gps_array_data[step] =',';
	step++;

	for (i = 0; i < 11 ; i++) {
		gps_array_data[step] = gpsdata.lng[i];
		step++;
	}

	gps_array_data[step] =',';
	step++;

	for (i = 0; i < 11 ; i++) {
		gps_array_data[step] = gpsdata.time[i];
		step++;
	}

	gps_array_data[step] =',';
	step++;

	for (i = 0; i < 3 ; i++) {
		gps_array_data[step] = gpsdata.sats[i];
		step++;
	}

	gps_array_data[step] =',';
	step++;

	for (i = 0; i < 9 ; i++) {
		gps_array_data[step] = gpsdata.alti[i];
		step++;
	}

	gps_array_data[step] =',';
	step++;

	gps_array_data[step] = gpsdata.ns;
	step++;
	gps_array_data[step] = gpsdata.ew;
	step++;
	gps_array_data[step] = gpsdata.valid;

}


/**
 * Sends a single byte out through UART
 **/
void UART_send_byte(unsigned char byte)
{

	while (!(IFG2&UCA0TXIFG));			// USCI_A0 TX buffer ready? //TODO: change this to avoid infinite loop!!!!!
	UCA0TXBUF = byte;					// TX -> RXed character

}

void puts(char *s) {
	char c;

	// Loops through each character in string 's'
	while (c = *s++) {
		UART_send_byte(c);
	}
}
/**
 * puts() is used by printf() to display or send a character. This function
 *     determines where printf prints to. For this case it sends a character
 *     out over UART.
 **/
void putc(unsigned b) {
	UART_send_byte(b);
}

// Low-Frequency Baud Rate Mode
// read chapter 15.3.10 of msp430x2xxx user's guide for baud rate generation
void UART_setup_9600(void)
{
	P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD, No need to set IO direction
	UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
	UCA0BR0 = 0x03;                           // 3 = INT(3.41), 32.768kHz/9600 = 3.41
	UCA0BR1 = 0x00;                           //
	UCA0MCTL = UCBRS1 + UCBRS0;               // Modulation UCBRSx = 3 = round(0.41 * 8)
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void UART_setup_4800(void)
{
	P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD, No need to set IO direction
	UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
	UCA0BR0 = 0x06;                           // 6 = INT(6.83), 32.768kHz/4800 = 6.83
	UCA0BR1 = 0x00;                           //
	UCA0MCTL = UCBRS2 + UCBRS1 + UCBRS0;      // Modulation UCBRSx = 7 = round(0.83 * 8)
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void UART_init(void)
{
	IE2 |= UCA0RXIE;                          // default: Enable USCI_A0 RX interrupt
	IE2 &= ~UCA0TXIE;						  // default: Disable USCI_A0 TX interrupt
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
	uart_byte = UCA0RXBUF;					// Get the received character

	// gps
	if(uart_byte == '$') //$ = Start of NMEA Sentence
	{
		gpsindex = 0;
		gpsdone = 0;
	}
	else if(uart_byte == '*') //<CR> = End of Transmission
	{
		gpsdone = 1;
		if(rawgps[4] == 'G') //Make sure this is a GGA sentence
		{
		   gps_parse_nmea();
		   gps_convert_stream();
		   gps_set_readyFlag();
		   __bic_SR_register_on_exit(LPM3_bits);
		}
	}
	if(gpsdone != 1)
	{
		rawgps[gpsindex] = uart_byte;
		gpsindex++;
	}
	if(gpsindex > 81)
	{
		gpsindex = 0;
	}

}

