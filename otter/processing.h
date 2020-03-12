#ifndef PROCESSING_H
#define PROCESSING_H

#include "draw.h"
#include "vga.h"

void grayscale(RGB_332_type *image, int overwrite);

void shiftColor(RGB_332_type *image, int deltaR, int deltaG, int deltaB);

void convolve(RGB_332_type *image, int kernel[3][3], int divisor);

void sobel(RGB_332_type *image, int threshold);

#endif
