/*
 * global.h
 *
 *  Created on: 2012. 6. 3.
 *      Author: donghee
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "ossitypes.h"

// Macros for turning on and off the LEDs (for testing only)
enum {FALSE = 0, TRUE = 1};
#define _BV(bit) (1<<(bit))

//macros for IO config (with preprocessor abuse)
#define st(x) do{x} while(__LINE__ == -1)

enum IoState {LOW = 0, HIGH = 1, TOGGLE = 2};
#define IO_SET(name,val)                _IO_SET(name##_PORT,name##_PIN,val)
#define _IO_SET(port,pin,val)           st(__IO_SET(port,pin,val);)
#define __IO_SET(port,pin,val)          if (val == TOGGLE)\
                                        {\
                                            st((P##port##OUT ^= _BV(pin)););\
                                        }\
                                        else if (val == HIGH)\
                                        {\
                                            st((P##port##OUT |= _BV(pin)););\
                                        }\
                                        else\
                                        {\
                                            st((P##port##OUT &= ~_BV(pin)););\
                                        }

enum IoDirection {INPUT = 0, OUTPUT = 1};
#define IO_DIRECTION(name,dir)          _IO_DIRECTION(name##_PORT,name##_PIN,dir)

#define _IO_DIRECTION(port,pin,dir)     st(__IO_DIRECTION(port,pin,dir);)
#define __IO_DIRECTION(port,pin,dir)    if (dir)\
                                        {\
                                            st((P##port##DIR |= _BV(pin)););\
                                        }\
                                        else\
                                       {\
                                            st((P##port##DIR &= ~_BV(pin)););\
                                        }

void delay_ms(uint16_t delay);
void min_max(uint8_t min, uint8_t max, uint8_t value);

#endif /* GLOBAL_H_ */


