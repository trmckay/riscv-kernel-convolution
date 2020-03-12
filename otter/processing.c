#include <math.h>
#include "processing.h"

int _clamp(int n, int min, int max)
{
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

RGB_332_type _grayscalePixel(RGB_332_type pixel)
{
    // isolate R, G, and B values
    RGB_332_type R = (pixel & 0b11100000) >> 5;
    RGB_332_type G = (pixel & 0b00011100) >> 2;
    RGB_332_type B = (pixel & 0b00000011);
    // average colors, but count blue twice since it has exactly half the range as R and G
    RGB_332_type intensity = (R+G+B+B)/3;
    RGB_332_type RGB = 0;
    RGB += intensity << 5;
    RGB += intensity << 2;
    // halve the intensity for blue since once again it only has half the range
    RGB += intensity >> 1;
    return RGB;
}

/* all pixel values in memory <- grayscale equivalent */
void grayscale(RGB_332_type *image, int overwrite)
{
    int offset = 0;
    pixel_buf_type pix_buf;

    for (int row = 0; row < Y_RES; row++)
    {
        for (int col = 0; col < X_RES; col++)
        {
            RGB_332_type newPixel = _grayscalePixel(*(image + offset));
            draw_dot(offset, newPixel, &pix_buf);
            offset++;
        }
    }
}

void shiftColor(RGB_332_type *image, int deltaR, int deltaG, int deltaB)
{
    int offset = 0;
    pixel_buf_type pix_buf;

    for (int row = 0; row < Y_RES; row++)
    {
        for (int col = 0; col < X_RES; col++)
        {
            // isolate R, G, and B values
            RGB_332_type RGB = *(image+offset);
            RGB_332_type R = (RGB & 0b11100000) >> 5;
            RGB_332_type G = (RGB & 0b00011100) >> 2;
            RGB_332_type B = (RGB & 0b00000011);

            // add the color delta
            // clamp it between 0 and the max intensity for that channel
            R = _clamp(R + deltaR, 0, 0b111);
            G = _clamp(G + deltaG, 0, 0b111);
            B = _clamp(B + deltaB, 0, 0b011);
            RGB = (R << 5) + (G << 2) + B;

            draw_dot(offset, RGB, &pix_buf);
            offset++;
        }
    }
}

// apply the kernel to the pixel matrix and divide the result by the divisor
RGB_332_type _applyKernel(RGB_332_type pixels[3][3], int kernel[3][3], int divisor)
{
    int rTotal = 0;
    int gTotal = 0;
    int bTotal = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // calculate the convolution separately for each color channel
            RGB_332_type RGB = pixels[i][j];
            rTotal += ((RGB & 0b11100000) >> 5) * kernel[2-i][2-j];
            gTotal += ((RGB & 0b00011100) >> 2) * kernel[2-i][2-j];
            bTotal += (RGB & 0b00000011) * kernel[2-j][2-i];
        }
    }
    // clamp values between 0 and the max intensity for that color channel
    RGB_332_type newR = (RGB_332_type)(_clamp(rTotal / divisor, 0, 0b111));
    RGB_332_type newG = (RGB_332_type)(_clamp(gTotal / divisor, 0, 0b111));
    RGB_332_type newB = (RGB_332_type)(_clamp(bTotal / divisor, 0, 0b011));

    RGB_332_type newRGB = (newR << 5) + (newG << 2) + (newB);
    return newRGB;
}

RGB_332_type _sobelKernel(RGB_332_type pixels[3][3], int threshold)
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
            RGB_332_type pixel = _grayscalePixel(pixels[i][j]);
            // consider the red channel, this is arbitrary
            RGB_332_type intensity = (pixel & 0b11100000) >> 5;
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
    RGB_332_type newRGB = (gradient << 5) + (gradient << 2) + (gradient & 0b011);
    return newRGB;
}

void sobel(RGB_332_type *image, int threshold)
{
    int offset = 0;
    pixel_buf_type pix_buf;

    for (int row = 0; row < Y_RES; row++)
    {
        for (int col = 0; col < X_RES; col++)
        {
            // get pointer to current pixel
            RGB_332_type *pixel = image + offset;
            // build matrix of pixel and its neighbors
            RGB_332_type pixels[3][3] = {
                { *(pixel - X_RES - 1), *(pixel - X_RES),   *(pixel - X_RES + 1) },

                { *(pixel - 1),         *(pixel),           *(pixel + 1) },

                { *(pixel + X_RES - 1), *(pixel + X_RES),   *(pixel + X_RES + 1) }
            };
            // apply the sobel operator
            RGB_332_type RGB = _sobelKernel(pixels, threshold);
            // draw the new value
            draw_dot(offset, RGB, &pix_buf);
            offset++;
        }
    }
}
/* kernel convolution using an arbitrary kernel and optional divisor to normalize output
see https://en.wikipedia.org/wiki/Kernel_(image_processing)#Convolution */
void convolve(RGB_332_type *image, int kernel[3][3], int divisor)
{
    int offset = 0;
    pixel_buf_type pix_buf;

    for (int row = 0; row < Y_RES; row++)
    {
        for (int col = 0; col < X_RES; col++)
        {
            RGB_332_type *pixel = image + offset;
            RGB_332_type pixels[3][3] = {
                { *(pixel - X_RES - 1), *(pixel - X_RES),   *(pixel - X_RES + 1) },

                { *(pixel - 1),         *(pixel),           *(pixel + 1) },

                { *(pixel + X_RES - 1), *(pixel + X_RES),   *(pixel + X_RES + 1) }
            };
            RGB_332_type RGB = _applyKernel(pixels, kernel, divisor);
            draw_dot(offset, RGB, &pix_buf);
            offset++;
        }
    }
}
