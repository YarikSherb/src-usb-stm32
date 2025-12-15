/*
 * strip.c
 *
 *  Created on: 24 ���. 2020 �.
 *      Author: Flexz
 */

// #include "stm32h7xx_hal.h"
#include "strip.h"
#include "strip_ll.h"

//rgb strip[CFG_STRIP_CHANNELS][CFG_STRIP_LEDS];
rgb strip[CFG_STRIP_LEDS];

void StripInit()
{
	reset_bits();
	StripClear(rgb_create(0,0,0));
	StripUpdate();
	StripLLInit(0,0);
}

void LedSet(int channel, int idx, rgb color)
{
	//strip[channel][idx] = color;
	strip[idx] = color;
}

void StripSetChannel(int channel, rgb *src, int cnt)
{
	if(cnt > CFG_STRIP_LEDS)
		cnt = CFG_STRIP_LEDS;
	for(int i = 0; i < cnt; i++)
	{
		LedSet(channel, i, src[i]);
	}
}

void StripSetLed(int channel, int led, rgb color)
{
	if(led >= CFG_STRIP_LEDS)
		return;
	if(led < 0)
		return;
	LedSet(channel, led, color);
}

rgb StripGetLed(int channel, int idx)
{
	//return strip[channel][idx];
	return strip[idx];
}

void StripClear(rgb color)
{
	for(int ch = 0; ch < CFG_STRIP_CHANNELS; ch++)
	{
		for(int idx = 0; idx < CFG_STRIP_LEDS; idx++)
		{
			LedSet(ch, idx, color);
		}
	}
}

void StripUpdate()
{
	//StripLLSet(0, strip[0], CFG_STRIP_LEDS);
	StripLLSet(0, &strip[0], CFG_STRIP_LEDS);
}
