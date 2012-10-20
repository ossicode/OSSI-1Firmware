#include "i2c.h"
#include "config.h"
#include "payload.h"

void I2C_PAYLOAD_Init(uint8_t slave_address)
{
	 _EINT();
	InitI2C(slave_address);
}

void I2C_PAYLOAD_Write(uint8_t value)
{
//  while (I2CDCTL&I2CBUSY); // I2C module busy
  while (I2CDCTL&I2CBB); // I2C bus busy. from TI forum http://goo.gl/Gsxsw http://www.ti.com/lit/er/slaz018d/slaz018d.pdf

  I2CBuffer[0] = value;
  PtrTransmit = 0;

  I2CWriteInit();
  I2CNDAT = 1;
  I2CTCTL |= I2CSTT+I2CSTP;   // start and stop condition generation
}

void I2C_PAYLOAD_Writes(CMD _cmd)
{
  while (I2CDCTL&I2CBB); // I2C bus busy. from TI forum http://goo.gl/Gsxsw http://www.ti.com/lit/er/slaz018d/slaz018d.pdf

  I2CBuffer[0] = _cmd.data;
  I2CBuffer[1] = _cmd.length;
  I2CBuffer[2] = _cmd.id;
  PtrTransmit = _cmd.length+1;

  I2CWriteInit();
  I2CNDAT = _cmd.length+2;
  I2CTCTL |= I2CSTT+I2CSTP;   // start and stop condition generation
}

void  Payload_Init(void)
{
	I2C_PAYLOAD_Init(0x48);
}

void Payload_Light_On(void)
{
  CMD c;
  c.id = LIGHT_ON;
  c.length = 1;
  c.data = 1;
  I2C_PAYLOAD_Writes(c);
}

void Payload_Light_Off(void)
{
  CMD c;
  c.id = LIGHT_OFF;
  c.length = 1;
  c.data = 0;
  I2C_PAYLOAD_Writes(c);
}
