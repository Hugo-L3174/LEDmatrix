#include <stdio.h>
#include "pico/stdlib.h"
#include "Driver_IS31FL3731.h"
#include "Drawing.h"

uint8_t frame_ = 0; ///< The frame (of 8) we are currently addressing

const uint8_t
  smile_bmp[] =
  { 0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100 },
  neutral_bmp[] =
  { 0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10111101,
    0b10000001,
    0b01000010,
    0b00111100 },
  frown_bmp[] =
  { 0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10011001,
    0b10100101,
    0b01000010,
    0b00111100 };

int main() {

    uint8_t sweep[] = {1, 2, 3, 4, 6, 8, 10, 15, 20, 25, 30, 35, 35, 30, 25, 20, 15, 10, 8, 6, 4, 3, 2, 1};

    initMatrix();
    while (1)
    {
        // animate over all the pixels, and set the brightness from the sweep table
        // for (uint8_t incr = 0; incr < 24; incr++)
        //     for (uint8_t x = 0; x < 16; x++)
        //         for (uint8_t y = 0; y < 9; y++)
        //             drawPixel(x, y, sweep[(x+y+incr)%24], 0);
        // for (uint8_t i = 0; i < 16; i++)
        // {
        //     drawPixel(i, 0, 1, 0);
        //     sleep_ms(20);
        // }
        // for (uint8_t i = 0; i < 16; i++)
        // {
        //     drawPixel(i, 0, 0, 0);
        //     sleep_ms(20);
        // }
        clear(frame_);
        Draw1bitBitmap(3, 0, smile_bmp, 8, 8, 16, frame_);
        sleep_ms(500);
        
        clear(frame_);
        Draw1bitBitmap(3, 0, neutral_bmp, 8, 8, 16, frame_);
        sleep_ms(500);

        clear(frame_);
        Draw1bitBitmap(3, 0, frown_bmp, 8, 8, 16, frame_);
        sleep_ms(500);
        
    }
    
    return 0;
}