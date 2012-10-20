/*
 * adc.h
 *
 *  Created on: 2012. 6. 3.
 *      Author: donghee
 */

#ifndef ADC_H_
#define ADC_H_

#include <msp430.h>

#define VSOLAR 4
#define VBUS 3
#define VBAT 2
#define VSUPERCAP 1

#define SOLAR_CS1 4
#define SOLAR_CS2 3
#define SOLAR_CS3 2
#define SOLAR_CS4 1
#define SOLAR_CS5 5
#define SUN_IN 6

#define A0_PORT 5
#define A0_PIN 5

#define A1_PORT 5
#define A1_PIN 6

#define A2_PORT 5
#define A2_PIN 7

void adc_init(void);
uint16_t adc_read(uint16_t channel);

uint16_t ADC_Get_Voltage_A0(uint8_t channel);
uint16_t ADC_Get_Voltage_A1(uint8_t channel);

#endif /* ADC_H_ */
