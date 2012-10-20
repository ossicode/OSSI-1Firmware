#include "i2c.h"

unsigned char I2CBuffer[3];
int PtrTransmit;
int PtrReceive;

int NACK_Count;

/*---------------------------------------------------------------------------*/
void InitI2C(uint8_t slave_address)
// Description:
//   Initialization of the I2C Module
{
  NACK_Count =0;

  P3SEL = 0x0A;          // select module function for the used I2C pins
  P3DIR &= ~0x0A;

  // Recommended initialisation steps of I2C module as shown in User Guide:
  U0CTL |= I2C+SYNC;     // (1) Select I2C mode with SWRST=1
  U0CTL &= ~I2CEN;       // (2) disable the I2C module
                         // (3) Configure the I2C module with I2CEN=0 :
                                // U0CTL default settings:
                                //   7-bit addressing, no DMA, no feedback
  I2CTCTL = I2CTRX+I2CSSEL_2;   // byte mode, repeat mode, clock source = SMCLK,
                                // transmit mode
  I2CSA = slave_address;         // define Slave Address
                                // In this case the Slave Address defines the
                                // control byte that is sent to the EEPROM.
//  I2CPSC = 0x00;                // I2C clock = clock source/1
//  I2CTCTL = I2CSSEL_1;                      // SMCLK

  I2CSCLH = 0x03;               // SCL high period = 5*I2C clock
  I2CSCLL = 0x03;               // SCL low period  = 5*I2C clock
  U0CTL |= I2CEN;        // (4) set I2CEN via software

}



void I2CWriteInit(void)
// Description:
//   Initialization of the I2C Module for Write operation.
{
  U0CTL |= MST;           // define Master Mode
  I2CTCTL |= I2CTRX;      // I2CTRX=1 => Transmit Mode (R/W bit = 0)
  I2CIFG &= ~TXRDYIFG;
  I2CIE = TXRDYIE;        // enable Transmit ready interrupt
}

/*---------------------------------------------------------------------------*/
void I2CReadInit(void)
// Description:
//   Initialization of the I2C Module for Read operation.
{
  I2CTCTL &= ~I2CTRX;     // I2CTRX=0 => Receive Mode (R/W bit = 1)
  I2CIE = RXRDYIE;        // enable Receive ready interrupt
}

//// I2C Interrupt Vector (I2CIV) handler
#pragma vector=USART0TX_VECTOR
__interrupt void I2C_ISR (void)
{
  switch( I2CIV )
  {
  case  2: break;                          // Arbitration lost
  case  4:
  {
	  break;                          // No Acknowledge
  }
  case  6: break;                          // Own Address
  case  8: break;                          // Register Access Ready
  case 10:
  {

      I2CBuffer[PtrReceive]=I2CDRB;   // store received data in buffer
      PtrReceive = PtrReceive-1;
      if (PtrReceive < 0)
      {
    	  I2CIE &= ~RXRDYIE;        // disable interrupts
      }
      break;
  }
   case 12:
   {
    I2CDRB = I2CBuffer[PtrTransmit];
    PtrTransmit = PtrTransmit-1;

    if (PtrTransmit <0)
    {
    	I2CIE &= ~TXRDYIE;        // disable interrupts
    }
    break;
    }
   case 14: break;                          // General Call
   case 16: break;                          // Start Condition
  }
}
