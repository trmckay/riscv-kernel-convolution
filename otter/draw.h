#ifndef DRAW_H
#define DRAW_H

#include "vga.h"

void drawImage(RGB_332_type *image);
void draw_dot(unsigned int offset, unsigned char RGB, pixel_buf_type *px_bfr);

#endif
