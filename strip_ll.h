/*
 * strip_ll.h
 *
 *  Created on: 28 Dec. 2020 y.
 *      Author: Flexz
 */

#ifndef STRIP_LL_H_
#define STRIP_LL_H_

#include "config.h"
#include "colors.h"

#define BITS_PER_LED	24
#define TOTAL_BITS		(BITS_PER_LED * CFG_STRIP_LEDS)

//48Mhz / (Prescaler+1) / Pulse
//24M / 1 / 30 = 800.000, Period - 1.25uS, 21nS per tick
#define T_PULSE	90U //
#define T_PRESCALER 0U

#define T_ONE	42U 
#define T_ZERO	18U 


void StripLLInit(unsigned char reset_b, unsigned char chan);
void StripLLSet(int channel, rgb *data, int cnt);
//void StripLLEnable(int enable);
void reset_bits(void);

#endif /* STRIP_LL_H_ */
