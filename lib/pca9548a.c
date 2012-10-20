#include "pca9548a.h"
#include "i2c.h"
#include "config.h"

void PCA9548A_Reset(void)
{
    IO_SET(PCA9548A_RESET, LOW);
    __delay_cycles(100000);
    IO_SET(PCA9548A_RESET, HIGH);
    __delay_cycles(100000);
}

void PCA9548A_Init(uint8_t slave_address)
{
	PCA9548A_Reset();
	 _EINT();
	InitI2C(slave_address);
}

void PCA9548A_SetChannel(uint8_t channel)
{
  uint8_t control_register;

  while (I2CDCTL&I2CBB);
  control_register = 1 << channel; // select channel
  I2CBuffer[0] = control_register;
  PtrTransmit = 0;

  I2CWriteInit();
  I2CNDAT = 1;
  I2CTCTL |= I2CSTT+I2CSTP;   // start and stop condition generation
}
