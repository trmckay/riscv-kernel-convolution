#include <math.h>
#include "draw.h"

typedef unsigned char color;

int _clamp(int n, int min, int max)
{
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

color _grayscalePixel(color pixel)
{
    // isolate R, G, and B values
    color R = (pixel & 0b11100000) >> 5;
    color G = (pixel & 0b00011100) >> 2;
    color B = (pixel & 0b00000011);
    // average colors, but count blue twice since it has exactly half the range as R and G
    color intensity = (R+G+B+B)/3;
    color RGB = 0;
    RGB += intensity << 5;
    RGB += intensity << 2;
    // halve the intensity for blue since once again it only has half the range
    RGB += intensity >> 1;
    return RGB;
}

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
            color newPixel = _grayscalePixel(*(image + offset));
            drawDot(col, row, newPixel);
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

            // add the color delta but clamp it between 0 and the max intensity for that channel
            R = _clamp(R + deltaR, 0, 0b111);
            G = _clamp(G + deltaG, 0, 0b111);
            B = _clamp(B + deltaB, 0, 0b011);
            RGB = (R << 5) + (G << 2) + B;

            drawDot(col, row, RGB);
            offset++;
        }
    }
}

// apply the kernel to the pixel matrix and divide the result by the divisor
color _applyKernel(color pixels[3][3], int kernel[3][3], int divisor)
{
    int rTotal = 0;
    int gTotal = 0;
    int bTotal = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // calculate the convolution separately for each color channel
            color RGB = pixels[i][j];
            rTotal += ((RGB & 0b11100000) >> 5) * kernel[2-i][2-j];
            gTotal += ((RGB & 0b00011100) >> 2) * kernel[2-i][2-j];
            bTotal += (RGB & 0b00000011) * kernel[2-j][2-i];
        }
    }
    // clamp values between 0 and the max intensity for that color channel
    color newR = (color)(_clamp(rTotal / divisor, 0, 0b111));
    color newG = (color)(_clamp(gTotal / divisor, 0, 0b111));
    color newB = (color)(_clamp(bTotal / divisor, 0, 0b011));

    color newRGB = (newR << 5) + (newG << 2) + (newB);
    return newRGB;
}

color _sobelKernel(color pixels[3][3], int threshold)
{
    // kernels used for sobel operator
    int Gx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int Gy[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };

    int dx = 0;
    int dy = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // sobel must operate on a grayscale image
            color pixel = _grayscalePixel(pixels[i][j]);
            // consider the red channel, this is arbitrary
            color intensity = (pixel & 0b11100000) >> 5;
            /* apply both kernels to get approximations of the partial derivative
               in the x and y directions */
            dx += intensity * Gx[2-i][2-j];
            dy += intensity * Gy[2-i][2-j];
        }
    }

    // taking the magnitude of the partials gives the gradient
    int gradient = sqrt(dx*dx + dy*dy);
    // gradients that do not meet the threshold will not be considered edges
    if (gradient < threshold) gradient = 0;
    // clamp the gradient at 8
    gradient = _clamp(gradient, 0, 0b111);
    // shift the gradient into all three color channels
    color newRGB = (gradient << 5) + (gradient << 2) + (gradient & 0b011);
    return newRGB;
}

void sobel(color *image, int dim, int threshold)
{
    // for each pixel
    int offset = 0;
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            // get pointer to current pixel
            color *pixel = image + offset;
            // build matrix of pixel and its neighbors
            color pixels[3][3] = {
                { *(pixel - dim - 1),   *(pixel - dim),   *(pixel - dim + 1) },

                { *(pixel - 1),         *(pixel),         *(pixel + 1) },

                { *(pixel + dim - 1),   *(pixel + dim),   *(pixel + dim + 1) }
            };
            // apply the sobel operator
            color RGB = _sobelKernel(pixels, threshold);
            // draw the new value
            drawDot(col, row, RGB);
            offset++;
        }
    }
}

/* kernel convolution using an arbitrary kernel and optional divisor to normalize output
see https://en.wikipedia.org/wiki/Kernel_(image_processing)#Convolution */
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
            color RGB = _applyKernel(pixels, kernel, divisor);
            drawDot(col, row, RGB);
            offset++;
        }
    }
}
