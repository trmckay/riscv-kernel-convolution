// #include <stdio.h>
// #include <math.h>
#include "draw.h"

typedef unsigned char color;

/*
IMPORTANT!
Most if not all image processing functions assume a SQUARE image.
This is not a fundamental limitation of the algorithms,
but a technique to save time and memory.
It is costly to calculate the length of a rectangular image due to the lack of
hardware multiplication on the OTTER.
*/

/* all pixel values in memory <- grayscale equivalent */
void grayscale(color *image, int dim, int overwrite)
{
    // for each pixel
    // (end - image) = length of array
    int offset = 0;
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            // isolate R, G, and B values
            color RGB = *(image+offset);
            color R = (RGB & 0b11100000) >> 5;
            color G = (RGB & 0b00011100) >> 2;
            color B = (RGB & 0b00000011);
            color intensity = (R+G+B+B)/3;
            RGB = 0;
            RGB += intensity << 5;
            RGB += intensity << 2;
            RGB += intensity >> 1;
            if (overwrite > 0) {
                *(image + offset) = RGB;
            }
            drawDot(col, row, RGB);
            offset++;
        }
    }
}

int clamp(int n, int min, int max)
{
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

void shiftColor(color *image, int dim, int deltaR, int deltaG, int deltaB)
{
    // for each pixel
    // (end - image) = length of array
    int offset = 0;
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            // isolate R, G, and B values
            color RGB = *(image+offset);
            color R = (RGB & 0b11100000) >> 5;
            color G = (RGB & 0b00011100) >> 2;
            color B = (RGB & 0b00000011);

            R = clamp(R + deltaR, 0, 0b111);
            G = clamp(G + deltaG, 0, 0b111);
            B = clamp(B + deltaB, 0, 0b011);
            RGB = (R << 5) + (G << 2) + B;

            drawDot(col, row, RGB);
            offset++;
        }
    }
}

color applyKernel(color pixels[3][3], int kernel[3][3], int divisor)
{
    int rTotal = 0;
    int gTotal = 0;
    int bTotal = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            color RGB = pixels[i][j];
            rTotal += ((RGB & 0b11100000) >> 5) * kernel[2-i][2-j];
            gTotal += ((RGB & 0b00011100) >> 2) * kernel[2-i][2-j];
            bTotal += (RGB & 0b00000011) * kernel[2-j][2-i];
        }
    }
    color newR = (color)(clamp(rTotal / divisor, 0, 0b111));
    color newG = (color)(clamp(gTotal / divisor, 0, 0b111));
    color newB = (color)(clamp(bTotal / divisor, 0, 0b011));

    color newRGB = (newR << 5) + (newG << 2) + (newB);
    return newRGB;
}

void convolve(color *image, int dim, int kernel[3][3], int divisor)
{
    int offset = 0;
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            color *pixel = image + offset;
            color pixels[3][3] = {
                { *(pixel - dim - 1),   *(pixel - dim),   *(pixel - dim + 1) },

                { *(pixel - 1),         *(pixel),         *(pixel + 1) },

                { *(pixel + dim - 1),   *(pixel + dim),   *(pixel + dim + 1) }
            };
            color RGB = applyKernel(pixels, kernel, divisor);
            drawDot(col, row, RGB);
            offset++;
        }
    }
}
