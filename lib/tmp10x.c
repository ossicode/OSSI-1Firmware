#include "24lc256.h"
#include "i2c.h"

void TMP10x_Init(uint8_t slave_address)
{
	 _EINT();
	InitI2C(slave_address);
}


uint16_t TMP10x_Read()
{
  uint8_t point_register;
  uint16_t tempature;

//  while (I2CDCTL&I2CBUSY);  // wait until I2C module has finished all operations
  while (I2CDCTL&I2CBB);

  point_register = 0; // Temperature Register
  I2CBuffer[0] = point_register;
  PtrTransmit = 0;

  I2CWriteInit();
  I2CNDAT = 1;
  I2CIFG &= ~ARDYIFG;
  I2CTCTL |= I2CSTT;
  while ((~I2CIFG)&ARDYIFG);  // wait untill transmission is finished

  I2CReadInit();
  I2CNDAT = 2;         // 2 byte should be received
  PtrReceive = 1;

  I2CIFG &= ~ARDYIFG;         // clear Access ready interrupt flag
  I2CTCTL |= I2CSTT+I2CSTP;   // start receiving and finally generate
                                //                  re-start and stop condition
  while ((~I2CIFG)&ARDYIFG);  // wait untill transmission is finished
  tempature = I2CBuffer[1];
  tempature = (tempature << 4)|(I2CBuffer[0]>>4);
  return  tempature;
}
