/*
 * colors.h
 *
 *  Created on: 24 ���. 2020 �.
 *      Author: Flexz
 */

#ifndef COLORS_H_
#define COLORS_H_

typedef struct {
    float r;       // a fraction between 0 and 1
    float g;       // a fraction between 0 and 1
    float b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    float h;       // angle in degrees
    float s;       // a fraction between 0 and 1
    float v;       // a fraction between 0 and 1
} hsv;

hsv rgb2hsv(rgb in);
rgb hsv2rgb(hsv in);

rgb rgb_create(float r, float g, float b);

#endif /* COLORS_H_ */
