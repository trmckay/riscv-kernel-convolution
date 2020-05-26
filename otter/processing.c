#include <math.h>
#include "processing.h"

int _clamp(int n, int min, int max)
{
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

#define VEC_MAG(A, B)\
    ((A) > (B)) ? ((A) + ((B)>>1)) : ((B) + ((A)>>1))

RGB_332_type _grayscale_pixel(RGB_332_type pixel)
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
    for (int row = 0; row < Y_RES; row++)
    {
        for (int col = 0; col < X_RES; col++)
        {
            RGB_332_type RGB = _grayscale_pixel(*(image + offset));
            DRAW_DOT(RGB);
            offset++;
        }
    }
}

void shift_color(RGB_332_type *image, int d_rd, int d_gr, int d_bl)
{
    int offset = 0;
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
            R = _clamp(R + d_rd, 0, 0b111);
            G = _clamp(G + d_gr, 0, 0b111);
            B = _clamp(B + d_bl, 0, 0b011);
            RGB = (R << 5) + (G << 2) + B;

            DRAW_DOT(RGB);
            offset++;
        }
    }
}

// apply the kernel to the pixel matrix and divide the result by the divisor
RGB_332_type _apply_kernel(RGB_332_type pixels[3][3], int kernel[3][3], int divisor)
{
    int rd_sum = 0;
    int gr_sum = 0;
    int bl_sum = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // calculate the convolution separately for each color channel
            RGB_332_type RGB = pixels[i][j];
            rd_sum += ((RGB & 0b11100000) >> 5) * kernel[2-i][2-j];
            gr_sum += ((RGB & 0b00011100) >> 2) * kernel[2-i][2-j];
            bl_sum += (RGB & 0b00000011) * kernel[2-j][2-i];
        }
    }
    // clamp values between 0 and the max intensity for that color channel
    RGB_332_type new_rd = (RGB_332_type)(_clamp(rd_sum >> divisor, 0, 0b111));
    RGB_332_type new_gr = (RGB_332_type)(_clamp(gr_sum >> divisor, 0, 0b111));
    RGB_332_type new_bl = (RGB_332_type)(_clamp(bl_sum >> divisor, 0, 0b011));

    RGB_332_type newRGB = (new_rd << 5) + (new_gr << 2) + (new_bl);
    return newRGB;
}

RGB_332_type _sobel_kernel(RGB_332_type pixels[3][3], int threshold)
{
    // kernels used for sobel operator
    int kx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int ky[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };

    int gx = 0, gy = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // sobel must operate on a grayscale image
            RGB_332_type pixel = _grayscale_pixel(pixels[i][j]);
            // consider the red channel, this is arbitrary
            RGB_332_type intensity = (pixel & 0b11100000) >> 5;
            /* apply both kernels to get approximations of the partial derivative
               in the x and y directions */
            gx += intensity * kx[2-i][2-j];
            gy += intensity * ky[2-i][2-j];
        }
    }

    // taking the magnitude of the partials gives the gradient
    int gradient = VEC_MAG(gx, gy);
    // gradients that do not meet the threshold will not be considered edges
    if (gradient < threshold) gradient = 0;
    // clamp the gradient at 8
    gradient = _clamp(gradient, 0, 0b111);
    // shift the gradient into all three color channels
    RGB_332_type new_rgb = (gradient << 5) + (gradient << 2) + (gradient & 0b011);
    return new_rgb;
}

void sobel(RGB_332_type *image, int threshold)
{
    coord_type offset = 0;
    for (coord_type row = 0; row < Y_RES; row++)
    {
        for (coord_type col = 0; col < X_RES; col++)
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
            RGB_332_type RGB = _sobel_kernel(pixels, threshold);
            // draw the new value
            DRAW_DOT(RGB);
            offset++;
        }
    }
}
/* kernel convolution using an arbitrary kernel and optional divisor to normalize output
see https://en.wikipedia.org/wiki/Kernel_(image_processing)#Convolution */
void convolve(RGB_332_type *image, int kernel[3][3], int divisor)
{
    coord_type offset = 0;
    for (coord_type row = 0; row < Y_RES; row++)
    {
        for (coord_type col = 0; col < X_RES; col++)
        {
            RGB_332_type *pixel = image + offset;
            RGB_332_type pixels[3][3] = {
                { *(pixel - X_RES - 1), *(pixel - X_RES),   *(pixel - X_RES + 1) },

                { *(pixel - 1),         *(pixel),           *(pixel + 1) },

                { *(pixel + X_RES - 1), *(pixel + X_RES),   *(pixel + X_RES + 1) }
            };
            RGB_332_type RGB = _apply_kernel(pixels, kernel, divisor);
            DRAW_DOT(RGB);
            offset++;
        }
    }
}
