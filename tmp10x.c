#include "24lc256.h"
#include "i2c.h"

void TMP10x_Init(uint8_t slave_address)
{
	InitI2C(slave_address);
}


uint16_t TMP10x_Read()
{
  while (I2CDCTL&I2CBUSY);  // wait until I2C module has finished all operations
  I2CReadInit();
  I2CNDAT = 1;         // 1 byte should be received
  while ((~I2CIFG)&ARDYIFG);  // wait untill transmission is finished
  return I2CBuffer[0];
}