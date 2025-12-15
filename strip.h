/*
 * strip.h
 *
 *  Created on: 24 ���. 2020 �.
 *      Author: Flexz
 */

#ifndef STRIP_H_
#define STRIP_H_

#include "colors.h"

void StripInit();

/**
 * Set specified channel colors
 * @param channel
 * @param src
 * @param cnt
 */
void StripSetChannel(int channel, rgb *src, int cnt);

void StripSetLed(int channel, int led, rgb color);

rgb StripGetLed(int channel, int idx);

/**
 * Set all channels/leds to the same color
 */
void StripClear(rgb color);

/**
 * Translate front buffer to color memory
 */
void StripUpdate();

#endif /* STRIP_H_ */
