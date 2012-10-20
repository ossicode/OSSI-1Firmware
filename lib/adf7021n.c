#include "adf7021n.h"
#include "global.h"

////////////// TX

#define TX_TXCLK_PORT 2
#define TX_TXCLK_PIN 3

#define TX_DATA_PORT 2
#define TX_DATA_PIN 4

#define TX_SCLK_PORT 2
#define TX_SCLK_PIN 5

#define TX_SREAD_PORT 2
#define TX_SREAD_PIN 6

#define TX_SDATA_PORT 2
#define TX_SDATA_PIN 7

#define TX_SLE_PORT 4
#define TX_SLE_PIN 0

#define TX_CE_PORT 4
#define TX_CE_PIN 1

#define TX_ON_PORT 4
#define TX_ON_PIN 2

///////////// TX PA

#define PA_ON_PORT 4
#define PA_ON_PIN 4

///////////// RX

#define RX_TXCLK_PORT 1
#define RX_TXCLK_PIN 2

#define RX_DATA_PORT 1
#define RX_DATA_PIN 3

#define RX_SWD_PORT 1
#define RX_SWD_PIN 4

#define RX_SCLK_PORT 1
#define RX_SCLK_PIN 5

#define RX_SREAD_PORT 1
#define RX_SREAD_PIN 6

#define RX_SDATA_PORT 1
#define RX_SDATA_PIN 7

#define RX_SLE_PORT 2
#define RX_SLE_PIN 0

#define RX_CE_PORT 2
#define RX_CE_PIN 1

#define RX_ON_PORT 4
#define RX_ON_PIN 3

// crystal 19.68MHz
// PFD 19.68MHz
// 7021-n: 433MHz 1200kbps
// vco bias: 2.75mA
// Xtal bias: 30uA
// PA bias: 9uA
// Desired Deviation: 2.102kHz
// Data Rate: 1.2 kbps
// IF BandWidth: 18.5kHz
// Power: 12.04dBm
static const uint32_t adf7021_regs[] = {
	0x095FF380, //r0
	0x00575011, //r1
//	0x0077F082, //r2 // 12.04dBm: PA setting 63
	0x0071B082, //r2 // 1dBm: PA setting 13
	0x37160123, //r3
	0x80293814, //r4
	0x00003155, //r5
};

unsigned char ShiftReg;

#define bitSet(value, bit) ((value) |= (0x01 << (bit)))
#define bitClear(value, bit) ((value) &= ~(0x01 << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define setShiftRegLSB(bitValue) (bitWrite(ShiftReg, 0, bitValue ))

char preamble_count;
unsigned char preamble_found;

volatile uint8_t adf702x_buf[RF_MAX] =  {0xAA, 0xAA, 0xAA, 0xAA, SYNC_WORD1, SYNC_WORD2, 0xAA, 0xAA, 0xAA, '1', '2', '3', '4', '5', '6', 0xAA, 0xAA, 0XAA};
volatile uint8_t adf702x_rx_buf[RF_MAX];

static const uint32_t adf7021_tx_reg = 0x01600850;
static unsigned char mode;

int bits_step =7;
int bytes_step = 0;


void spi_write(unsigned char _register, unsigned char mode)
{
	volatile int i;

	if (mode == TX) {
		for(i = 7; i >= 0; i--)
		{
			IO_SET(TX_SCLK, LOW);
			if(_register & (1<<i)) {
				IO_SET(TX_SDATA, HIGH);
			}
			else {
				IO_SET(TX_SDATA, LOW);
			}
			__delay_cycles(5);
			IO_SET(TX_SCLK, HIGH);
			__delay_cycles(15);
		}
		IO_SET(TX_SCLK, LOW);
	} else if (mode == RX) {
		for(i = 7; i >= 0; i--)
		{
			IO_SET(RX_SCLK, LOW);
			if(_register & (1<<i)) {
				IO_SET(RX_SDATA, HIGH);
			}
			else {
				IO_SET(RX_SDATA, LOW);
			}
			__delay_cycles(5);
			IO_SET(RX_SCLK, HIGH);
			__delay_cycles(15);
		}
		IO_SET(RX_SCLK, LOW);
	}
}

void adf702x_write(uint32_t registers, unsigned char mode)
{
	volatile int i;
	unsigned char _register;

	for(i=0;i<sizeof(registers);i++)
	{
		_register = (registers >> 24-(i*8)) & 0xff;
		spi_write(_register, mode);
	}
    // SLE
	if (mode == TX) {
		IO_SET(TX_SLE, HIGH);
		__delay_cycles(10);
		IO_SET(TX_SDATA, LOW);                       // SDATA low
		IO_SET(TX_SLE, LOW);
	} else if (mode == RX) {
		IO_SET(RX_SLE, HIGH);
		__delay_cycles(10);
		IO_SET(RX_SDATA, LOW);                       // SDATA low
		IO_SET(RX_SLE, LOW);
	}
	__delay_cycles(30);
}

void adf7021n_rx()
{

	IO_SET(TX_CE, LOW);
	IO_SET(RX_CE, HIGH);

	// fill adf702x_rx_buf
 	// r4 for 1200 bps
	adf702x_write(adf7021_regs[1], RX);
	adf702x_write(adf7021_regs[3], RX);
	adf702x_write(adf7021_regs[5], RX);
	adf702x_write(adf7021_regs[0], RX);
	adf702x_write(adf7021_regs[4], RX);

    //mode = RX;
    mode = IDLE;
}

void adf7021n_tx()
{
	IO_SET(RX_CE, LOW);
	IO_SET(TX_CE, HIGH);

	// fill adf702x_tx_buf
	adf702x_write(adf7021_regs[1], TX);
	adf702x_write(adf7021_regs[3], TX);
	adf702x_write(adf7021_tx_reg, TX);
	adf702x_write(adf7021_regs[2], TX);
	adf702x_write(adf7021_regs[4], TX);

	mode = TX;
}

unsigned char adf7021n_get_mode()
{
	return mode;
}

void adf7021n_enable_data_interrupt()
{

	// RX_TXCLK is 1.2
 	P1OUT |= BIT2; // pull up
	P1IE |= BIT2; // interrupt enable
	P1IES |= BIT2; // interrupt hi/lo falling edge
	P1IFG &= ~BIT2; // P1.2 IFG cleared just in case

	// TX_TXCLK is 2.3
 	P2OUT |= BIT3; // pull up
	P2IE |= BIT3; // interrupt enable
	P2IES |= BIT3; // interrupt hi/lo falling edge
	P2IFG &= ~BIT3; // P2.3 IFG cleared just in case

	// data communication check!
	P6DIR |= BIT0;
//	IO_DIRECTION TODO: change IO_DIRECTION
}

void adf7021n_recvStart()
{
	adf7021n_enable_data_interrupt();
	adf7021n_rx();
}

void TX_PA_PowerOn()
{
    IO_DIRECTION(PA_ON, OUTPUT);
    IO_SET(PA_ON, HIGH);
}

void adf7021n_setTxPaLevel()
{
	// TODO: make change level.
//	adf702x_write(adf7021_regs[2] , TX);
}

void adf7021n_sendStart()
{
	adf7021n_enable_data_interrupt();
	TX_PA_PowerOn();
	adf7021n_tx();
}

void adf7021n_init()
{
	IO_DIRECTION(TX_DATA, OUTPUT);
	IO_DIRECTION(TX_SCLK, OUTPUT);
	IO_DIRECTION(TX_SDATA, OUTPUT);
	IO_DIRECTION(TX_SLE, OUTPUT);
	IO_DIRECTION(TX_CE, OUTPUT);

	IO_DIRECTION(RX_DATA, INPUT);
	IO_DIRECTION(RX_SCLK, OUTPUT);
	IO_DIRECTION(RX_SDATA, OUTPUT);
	IO_DIRECTION(RX_SLE, OUTPUT);
	IO_DIRECTION(RX_CE, OUTPUT);

	// SCLK and SDATA pin must be LOW from start.
	IO_SET(TX_SCLK, LOW);
	IO_SET(TX_SDATA, LOW);
//	IO_SET(TX_SLE, LOW);

	IO_SET(RX_SCLK, LOW);
	IO_SET(RX_SDATA, LOW);
//	IO_SET(RX_SLE, LOW);

	// CE is HIGH
	IO_SET(RX_CE, HIGH);
	IO_SET(TX_CE, HIGH);
}


// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void adf7021n_Data_Tx_handler(void)
{
	switch (mode) {
		case TX:
			if(adf702x_buf[bytes_step] & (1<<bits_step)) {
				IO_SET(TX_DATA, HIGH);
			} else {
				IO_SET(TX_DATA, LOW);
			}

			bits_step--;

			if (bits_step < 0){bits_step = 7;bytes_step++;};
			if (bytes_step >= 15){
				bytes_step = 0; bits_step=7;
				P6OUT ^= BIT0;
			};

			break;
	}
	P2IFG &= ~BIT3; // P2.3 IFG cleared
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void adf7021n_Data_Rx_handler(void)
{
	switch (mode) {
		case RX:
			ShiftReg = ShiftReg << 1;
//			setShiftRegLSB(P5IN & BIT0);
			setShiftRegLSB(P1IN & BIT3);

		    bits_step++;

		    if (bits_step == 8) {
		    	bits_step = 0;
		    	switch(bytes_step){
		    		case 0:
		    			if (ShiftReg != SYNC_WORD2) {
		    				mode = IDLE;
		    			}
		    			break;
		    		case 1:
		    			// header 0
		    			break;
		    		case 2:
		    			// header 1
		    			break;
		    		case 3:
		    			// byte size
		    			break;
		    		default:
		    			adf702x_rx_buf[bytes_step-4] = ShiftReg;
		    	}

		        if(bytes_step > 4+6){ // 6 is data length
	        	  P6OUT ^= BIT0;
                  bytes_step = 0;
		          mode = IDLE;
		          // CE LOW?
		        }
		    	bytes_step++;

		    }
		    break;

		case IDLE:
			ShiftReg=ShiftReg<<1;
//			setShiftRegLSB(P5IN & BIT0); RX_DATA
			setShiftRegLSB(P1IN & BIT3);

			if (preamble_found == 1) {
				if (ShiftReg == SYNC_WORD1) {
					bits_step=0;
					bytes_step=0;
					preamble_count=0;
					preamble_found=0;
					mode = RX;
				}
				break;
			}

			if ((ShiftReg == VALID_PREAMBLE_BYTE_1) || (ShiftReg == VALID_PREAMBLE_BYTE_2)) {
				preamble_count++;
			} else {
				preamble_count = 0;
				preamble_found = 0; //false
			}

			if (preamble_count >= 16) {
				preamble_found = 1; //false
			}
			break;
	}
	P1IFG &= ~BIT2; // P1.2 IFG cleared
}
