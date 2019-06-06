#include "processing.h"

void sobel(unsigned char *image, int rows, int cols, int len)
{
    /* modified convolution routine to save memory:
     * sobel uses two different kernels and then takes the magnitude of the vector they form
     * the result of this forms the new pixel array
     * it's impractical to store both results from each kernel, so the magnitudes are
     * calculated as we go
     */

    grayscale(image, (image+len));

    int dxKernel[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int dyKernel[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    unsigned char *convolutions = (image + len);
    int offset = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            offset++;
            int intensity;
            int kernalVal;

            unsigned char *currPx = image + offset;
            unsigned char pixelMatrix[9] = {
                *(currPx - cols - 1),
                *(currPx - cols),
                *(currPx - cols + 1),
                *(currPx - 1),
                *(currPx),
                *(currPx + 1),
                *(currPx + cols - 1),
                *(currPx + cols + 1)
            };

            // run DX convolution (see 'convolve.c' for explanation)
            int dx = 0;
            // apply the kernel
            for (int j = 0; j < 9; j++)
            {
                // sample red value for intensity, this is arbitrary
                intensity = (int)((pixelMatrix[j] & 0b11100000) >> 5);
                kernalVal = dxKernel[8-j];
                dx += kernalVal * intensity;
            }

            // run DY convolution
            int dy = 0;
            // apply the kernel
            for (int j = 0; j < 9; j++)
            {
                // sample red value for intensity, this is arbitrary
                intensity = (int)((pixelMatrix[j] & 0b11100000) >> 5);
                kernalVal = dyKernel[8-j];
                dy += kernalVal * intensity;
            }

            // take the magnitude, normalize it, save it in new pixel array
            int magnitude = floorSqrt((dx * dx) + (dy * dy));
            unsigned char sobel = (unsigned char)(magnitude >> 29);
            *(image+len+offset) = sobel;
        }
    }

    // overwrite image with sobel result
    for (int i = 0; i < len; i++)
    {
        unsigned char intensity = *(image+len+i);
        unsigned char RGB = 0;
        RGB += intensity << 5;
        RGB += intensity << 2;
        RGB += intensity >> 1;
        *(image+i) = RGB;
    }
}
