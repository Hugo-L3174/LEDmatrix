#include "Drawing.h"

/**************************************************************************/
/*!
   @brief   Draw a RAM-resident 8-bit image (grayscale) at the specified (x,y)
   pos. Specifically for 8-bit display devices such as IS31FL3731; no color
   reduction/expansion is performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with grayscale bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Draw8bitBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint8_t frame) {
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      drawPixel(x + i, y, bitmap[j * w + i], frame);
    }
  }
}


/**************************************************************************/
/*!
   @brief      Draw a RAM-resident 1-bit image at the specified (x,y) position,
   using the specified foreground color (unset bits are transparent).
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Draw1bitBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint8_t frame) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = bitmap[j * byteWidth + i / 8];
      if (b & 0x80)
        drawPixel(x + i, y, color, frame);
    }
  }
}

// void Paint_DrawChar(int16_t x, int16_t y, const char Acsii_Char,
//                     sFONT* Font, int16_t Color_Foreground, int16_t Color_Background, uint8_t frame)
// {
//     int16_t Page, Column;

//     if (x > Paint.Width || y > Paint.Height) {
//         return;
//     }

//     uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
//     const unsigned char *ptr = &Font->table[Char_Offset];

//     for (Page = 0; Page < Font->Height; Page ++ ) {
//         for (Column = 0; Column < Font->Width; Column ++ ) {

//             //To determine whether the font background color and screen background color is consistent
//             if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
//                 if (*ptr & (0x80 >> (Column % 8)))
//                     drawPixel(x + Column, y + Page, Color_Foreground, frame);
//             } else {
//                 if (*ptr & (0x80 >> (Column % 8))) {
//                     drawPixel(x + Column, y + Page, Color_Foreground, frame);
//                 } else {
//                     drawPixel(x + Column, y + Page, Color_Background, frame);
//                 }
//             }
//             // One pixel is 8 bits
//             if (Column % 8 == 7)
//                 ptr++;
//         }// Write a line
//         if (Font->Width % 8 != 0)
//             ptr++;
//     }// Write all
// }