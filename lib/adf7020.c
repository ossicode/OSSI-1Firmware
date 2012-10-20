#include  "adf7020.h"
#include "global.h"

////////// TEST BOARD
//#define TXCLK_PORT 1
//#define TXCLK_PIN 2
//
//#define DATA_PORT 5
//#define DATA_PIN 0
//
//#define SCLK_PORT 5
//#define SCLK_PIN 1
//
//#define SREAD_PORT 5
//#define SREAD_PIN 2
//
//#define SDATA_PORT 5
//#define SDATA_PIN 3
//
//#define SLE_PORT 5
//#define SLE_PIN 4
//
//#define CE_PORT 5
//#define CE_PIN 5
//
//#define PAON_PORT 2
//#define PAON_PIN 7

////////// REAL BOARD
#define TXCLK_PORT 2
#define TXCLK_PIN 0

#define DATA_PORT 2
#define DATA_PIN 1

#define SCLK_PORT 2
#define SCLK_PIN 2

#define SREAD_PORT 2
#define SREAD_PIN 3

#define SDATA_PORT 2
#define SDATA_PIN 4

#define SLE_PORT 2
#define SLE_PIN 5

#define CE_PORT 2
#define CE_PIN 6

#define PAON_PORT 2
#define PAON_PIN 7


// crystal 11.05920MHz
//// 7020-1: 288.5/2 = 144.25 MHz
//static const uint32_t adf7020_1_regs[] = {
//	0x10d0b1f0, //r0
//	0x0002b011, //r1
//	0x800f00d2, //r2
//};

// crystal 19.2MHz
// PFD 6.4MHz
// 7020-1: 288.5/2 = 144.25 MHz
// vco bias 0.625mA
static const uint32_t adf7020_1_regs[] = {
	0x1168a000, //r0
	0x0002b031, //r1
	0x800f00d2, //r2
};


//#define PA_ON (63 << 9)  //-0.25 dbm = -16 + 0.45 * 35
#define PA_ON (10 << 9)  //-0.25 dbm = -16 + 0.45 * 35

#define TX 1
static unsigned char mode;

void spi_write(unsigned char _register)
{
	volatile int i;

	for(i = 7; i >= 0; i--)
	{
		IO_SET(SCLK, LOW);
		if(_register & (1<<i)) {
			IO_SET(SDATA, HIGH);
		}
		else {
			IO_SET(SDATA, LOW);
		}
		__delay_cycles(5);
		IO_SET(SCLK, HIGH);
		__delay_cycles(15);
	}
	IO_SET(SCLK, LOW);
}

void adf702x_write(uint32_t registers)
{
	volatile int i;
	unsigned char _register;

	for(i=0;i<sizeof(registers);i++)
	{
		_register = (registers >> 24-(i*8)) & 0xff;
		spi_write(_register);
	}
    // SLE
	IO_SET(SLE, HIGH);
	__delay_cycles(10);
	IO_SET(SDATA, LOW);                       // SDATA low
	IO_SET(SLE, LOW);

	__delay_cycles(30);

}


void ADF7020_1_Init()
{
	IO_DIRECTION(DATA,OUTPUT);
	IO_DIRECTION(SCLK,OUTPUT);
	IO_DIRECTION(SREAD,OUTPUT);
	IO_DIRECTION(SDATA,OUTPUT);
	IO_DIRECTION(SLE,OUTPUT);
	IO_DIRECTION(CE,OUTPUT);

	// SCLK and SDATA pin must be LOW from start.
	IO_SET(DATA, LOW);
	IO_SET(SCLK, LOW);
	IO_SET(SDATA, LOW);
	IO_SET(SLE, LOW);

}


void adf7020_1_tx()
{
	IO_SET(CE, HIGH);
	IO_DIRECTION(DATA,OUTPUT);
	__delay_cycles(1000);

	adf702x_write(adf7020_1_regs[1]); // once again for stable operation.
	__delay_cycles(4000);
	adf702x_write(adf7020_1_regs[0]);
	__delay_cycles(400);
	adf702x_write(adf7020_1_regs[2]);

	mode = TX;
}

void adf7020_1_poweramp(uint8_t is_pa_on)
{
	IO_SET(DATA, LOW);

	if (is_pa_on==1) {
		adf702x_write(adf7020_1_regs[2]|PA_ON);
//		IO_SET(PAON, HIGH);
	}
	else {
		adf702x_write(adf7020_1_regs[2]);
//		IO_SET(PAON, LOW);
	}
}


void ADF7020_1_SendStart()
{
	adf7020_1_tx();
	__delay_cycles(1000);
	adf7020_1_poweramp(1); // power on
	__delay_cycles(1000);
}

void adf7020_1_ook(uint8_t is_high)
{
	if (is_high == 1) {
		IO_SET(DATA, HIGH);
	}
	else {
		IO_SET(DATA, LOW);
	}
	mode = TX;
}
