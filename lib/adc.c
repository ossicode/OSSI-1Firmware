/*
 * adc.c
 *  Created on: 2012. 6. 3.
 *      Author: donghee
 */

#include <msp430.h>

#include "global.h"
#include "adc.h"


void ADC_Init(void)
{
  P6SEL |= (BIT0+BIT1);                            // P6.0 6.1 ADC option select

  ADC12CTL0 = ADC12ON+SHT0_2; // Turn on and set up ADC12, Vcc & Vss as reference
  ADC12CTL1 = SHP;                          // Use sampling timer
  ADC12MCTL0 = SREF0;                      // Vcc & Vss as reference
}

unsigned int ADC_Read(uint8_t channel)
{
  int voltage;

  ADC_Init();
  ADC12MCTL0 |= channel & 0x0F;
  __delay_cycles(3600);
  ADC12CTL0 |= ENC;                         // Enable conversions
  ADC12CTL0 |= ADC12SC;                   // Start conversion
  while ((ADC12IFG & BIT0)==0);
  voltage = ADC12MEM0;
  ADC12MCTL0 &= ~channel;            // clear the channel

  return voltage & 0xfff ;
}

void ADG708_Set_Init()
{
  IO_DIRECTION(A0, OUTPUT);
  IO_DIRECTION(A1, OUTPUT);
  IO_DIRECTION(A2, OUTPUT);
}

void ADG708_Set_Channel(uint8_t channel)
{
  uint8_t a0;
  uint8_t a1;
  uint8_t a2;

  a0 = (channel-1) & 1;
  a1 = ((channel-1)>>1) & 1;
  a2 = ((channel-1)>>2) & 1;
  IO_SET(A0, a0);
  IO_SET(A1, a1);
  IO_SET(A2, a2);
}

uint16_t ADC_Get_Voltage_A0(uint8_t channel)
{
  ADG708_Set_Init();
  ADG708_Set_Channel(channel);
  __delay_cycles(3600);
  return ADC_Read(0);
}

uint16_t ADC_Get_Voltage_A1(uint8_t channel)
{
  ADG708_Set_Init();
  ADG708_Set_Channel(channel);
  __delay_cycles(3600);
  return ADC_Read(1);
}
