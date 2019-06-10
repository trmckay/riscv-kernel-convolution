#include <stdio.h>
#include <stdlib.h>
// #include <math.h>
#include "draw.h"

typedef unsigned char   color;

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

            R = (R + deltaR) & 0b111;
            G = (G + deltaG) & 0b111;
            B = (B + deltaB) & 0b011;
            RGB = (R << 5) + (G << 2) + B;

            drawDot(col, row, RGB);
            offset++;
        }
    }
}

void averageBlur(color *image, int dim)
{
    for (int row = 1; row < dim-1; row++)
    {
        for (int col = 1; col < dim-1; col++)
        {
            color *pixel = image + (row * col) + col;

            color pixels[3][3] = {
                { *(pixel - dim - 1),   *(pixel - dim),   *(pixel - dim + 1) },

                { *(pixel - 1),         *(pixel),         *(pixel + 1) },

                { *(pixel + dim - 1),   *(pixel + dim),   *(pixel + dim + 1) }
            };

            int rTotal = 0; int gTotal = 0; int bTotal = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; i++) {
                    rTotal += (pixels[i][j] & 0b11100000) >> 5;
                    gTotal += (pixels[i][j] & 0b00011100) >> 2;
                    bTotal += (pixels[i][j] & 0b00000011);
                }
            }
            color R = rTotal / 9; color G = gTotal / 9; color B = bTotal / 9;
            color RGB = (R << 5) + (G << 2) + B;
            drawDot(col, row, RGB);
        }
    }
}

/* algorithm for kernel convolution:
 * [ a, b, c ]   [ r, s, t ]
 * | d, e, f | * | u, v, w | = (a*z) + (b*y) + (c*x) + (d*w) ... + (j*r)
 * [ h, i, j ]   [ x, y, z ]
*/

/*
int _sobelKernel(color *pixel, int dim, int kernel[3][3])
{
    color pixels[3][3] = {
        { *(pixel - dim - 1),   *(pixel - dim),   *(pixel - dim + 1) },

        { *(pixel - 1),         *(pixel),         *(pixel + 1) },

        { *(pixel + dim - 1),   *(pixel + dim),   *(pixel + dim + 1) }
    };

    int convolution = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            convolution += ((pixels[row][col] & 0b11100000) >> 5) * kernel[3-row][3-col];
        }
    }
    return convolution;
}

void sobel(color *image, int dim)
{
    int len = dim*dim;

    grayscale(image, dim, 1);

    int gx[3][3] = {
        {-1,  0,  1},
        {-2,  0,  2},
        {-1,  0,  1}
    };

    int gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    int offset;
    for (int row = 1; row < dim-1; row++)
    {
        for (int col = 1; col < dim-1; col++)
        {
            offset = (row * col) + col;

            int dx = _sobelKernel(image + offset, dim, gx);
            int dy = _sobelKernel(image + offset, dim, gy);

            unsigned int magnitude = (unsigned int) sqrt((dx*dx) + (dy*dy));
            color intensity = magnitude >> 2;

            color R = intensity << 5;
            color G = intensity << 2;
            color B = intensity >> 1;
            color RGB = R + G + B;

            drawDot(col, row, RGB);
        }
    }
}
*/
