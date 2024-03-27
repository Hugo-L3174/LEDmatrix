#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "Driver_IS31FL3731.h"
#include "fonts.h"

void Draw8bitBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint8_t frame);
void Draw1bitBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint8_t frame);


#endif