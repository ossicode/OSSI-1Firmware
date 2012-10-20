#include "24lc256.h"
#include "i2c.h"

void EEPROM_Init(uint8_t slave_address)
{
	 _EINT();
	InitI2C(slave_address);
}

/*---------------------------------------------------------------------------*/
void EEPROM_ByteWrite(unsigned int Address, unsigned char Data)
// Description:
//   Byte Write Operation. The communication via the I2C bus with an EEPROM
//   (2465) is realized. A data byte is written into a user defined address.
{
  unsigned char adr_hi;
  unsigned char adr_lo;

//  while (I2CDCTL&I2CBUSY); // wait until I2C module has finished all operations
  while (I2CDCTL&I2CBB); //http://www.ti.com/lit/er/slaz018d/slaz018d.pdf

  adr_hi = Address >> 8;         // calculate high byte
  adr_lo = Address & 0xFF;       //     and low byte of address

  I2CBuffer[2] = adr_hi;         // store single bytes that have to be sent
  I2CBuffer[1] = adr_lo;         //   in the I2CBuffer.
  I2CBuffer[0] = Data;
  PtrTransmit = 2;               // set I2CBuffer Pointer

  I2CWriteInit();
  I2CNDAT = 3;           // 1 control byte + 3 bytes should be transmitted
  I2CTCTL |= I2CSTT+I2CSTP;   // start and stop condition generation
                              //      => I2C communication is started
}

void EEPROM_AckPolling(void)
// Description:
// Acknowledge Polling. The EEPROM will not acknowledge if a write cycle is
// in progress. It can be used to determine when a write cycle is completed.
{
	unsigned int count;
	while (I2CDCTL&I2CBB); // wait until I2C module has finished all operations
	U0CTL &= ~I2CEN; // clear I2CEN bit => necessary to re-configure I2C module
	I2CTCTL |= I2CRM; // transmission is software controlled
	U0CTL |= I2CEN; // enable I2C module
	I2CIFG = NACKIFG; // set NACKIFG
	while (NACKIFG & I2CIFG)
	{
	I2CIFG=0x00; // clear I2C interrupt flags
	U0CTL |= MST; // define Master Mode
	I2CTCTL |= I2CTRX; // I2CTRX=1 => Transmit Mode (R/W bit = 0)

	_DINT(); // **** disable interrupts. following code lines should not be interrupted (time critical!!!)

	I2CTCTL |= I2CSTT; // start condition is generated
	while (I2CTCTL&I2CSTT); // wait till I2CSTT bit was cleared
	I2CTCTL |= I2CSTP; // stop condition is generated after slave address was sent
	// => I2C communication is started

	_EINT(); //**** enable interrupts again

	while (I2CDCTL&I2CBB); // wait till stop bit is reset
	}

	U0CTL &= ~I2CEN; // clear I2CEN bit => necessary to re-configure I2C module
	I2CTCTL &= ~I2CRM; // transmission is by the I2C module
	U0CTL |= I2CEN; // enable I2C module

	return;
}

unsigned char EEPROM_RandomRead(unsigned int Address)
// Description:
//   Random Read Operation. Data is read from the EEPROM. The EEPROM
//   address is defined with the parameter Address.
{
  unsigned char adr_hi;
  unsigned char adr_lo;

  while (I2CDCTL&I2CBB); //http://www.ti.com/lit/er/slaz018d/slaz018d.pdf

  adr_hi = Address >> 8;         // calculate high byte
  adr_lo = Address & 0xFF;       //     and low byte of address

  I2CBuffer[1] = adr_hi;         // store single bytes that have to be sent
  I2CBuffer[0] = adr_lo;         //   in the I2CBuffer.
  PtrTransmit = 1;               // set I2CBuffer Pointer

  I2CWriteInit();
  I2CNDAT = 2;         // 1 control byte + 2 bytes should be transmitted
  I2CIFG &= ~ARDYIFG;  // clear Access ready interrupt flag
  I2CTCTL |= I2CSTT;   // start condition generation
                       //      => I2C communication is started
  while ((~I2CIFG)&ARDYIFG);  // wait untill transmission is finished
  I2CReadInit();
  I2CNDAT = 1;         // 1 byte should be received
  PtrReceive = 0;

  I2CIFG &= ~ARDYIFG;         // clear Access ready interrupt flag
  I2CTCTL |= I2CSTT+I2CSTP;   // start receiving and finally generate
                              //                  re-start and stop condition
  while ((~I2CIFG)&ARDYIFG);  // wait untill transmission is finished
  return I2CBuffer[0];
}


/*---------------------------------------------------------------------------*/
unsigned char EEPROM_CurrentAddressRead(void)
// Description:
//   Current Address Read Operation. Data is read from the EEPROM. The current
//   address from the EEPROM is used.
{
//  while (I2CDCTL&I2CBUSY); // wait until I2C module has finished all operations
  while (I2CDCTL&I2CBB); //http://www.ti.com/lit/er/slaz018d/slaz018d.pdf

  I2CReadInit();
  U0CTL |= MST;         // define Master Mode
  I2CNDAT = 1;          // 1 byte should be received
  I2CIFG &= ~ARDYIFG;         // clear Access ready interrupt flag
  I2CTCTL |= I2CSTT+I2CSTP;   // start receiving and finally generate
                              //              re-start and stop condition
  while ((~I2CIFG)&ARDYIFG);  // wait untill transmission is finished
  return I2CBuffer[0];
}
