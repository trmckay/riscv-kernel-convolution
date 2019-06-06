#include <stdio.h>


int multiply(int a, int b)
{
    int product = b;
    if (b < a) {
        a = a^b;
        b = b^a;
        a = a^b;
    }
    for (int i = 1; i < a; i++) {
        product += b;
    }
    return product;
}

int floorDivide(int dividend, int divisor)
{
    int quotient = 0;
    int counter = dividend;
    while (counter > 0) {
        counter -= divisor;
        quotient += 1;
    }
    return quotient;
}

/* image is the beginning of the image array
 * matrix is pointer to beginning of a 3x3 kernel matrix
 * convolves a kernel accross a image referenced by 'image'
 * saves the result of each convolution as an array of unsigned chars at 'dest'
 * ONLY WORKS ON GRAYSCALE IMAGES
 * convolutions with color require triple the memory, which is not available on the OTTER
 */
void convolve(unsigned char *image, int len, int cols, int *kernel, unsigned char *dest, int divisor)
{
    // for each pixel in the array
    for (int i = 0; i < len; i++)
    {
        /*
         * collect pixel its neighbors in an array
         * image+i is a pointer to the current pixel
         * pixelMatrix = [a, b, c, d, e, f, g, h, i]
         * where e is the current pixel, *(image+i) and pixels are arranged as
         *  _______
         * | a b c |
         * | d e f |
         * | g h i |
         *  ‾‾‾‾‾‾‾
        */
        unsigned char *currPx = image + i;
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
        unsigned char convolution = 0;
        int intensity;
        int kernelVal;
        // apply the kernel
        for (int j = 0; j < 9; j++)
        {
            // sample red value for intensity, this is arbitrary
            intensity = (int)((pixelMatrix[j] & 0b11100000) >> 5);
            kernelVal = *(kernel+8-j);
            convolution += multiply(intensity, kernelVal);
        }
        // save the result (sum) in memory
        if (divisor != 0 && divisor != 1) convolution = convolution/divisor;
        *(dest+i) = convolution;
    }
}

/* all pixel values in memory <- grayscale equivalent
arguments:
a0: start address of image
a1: end of image
*/
void grayscale(unsigned char *image, unsigned char *end)
{
    // for each pixel
    // (end - image) = length of array
    for (int i = 0; i < (end-image); i++)
    {
        // isolate R, G, and B values
        unsigned char RGB = *(image+i);
        unsigned char R = (RGB & 0b11100000) >> 5;
        unsigned char G = (RGB & 0b00011100) >> 2;
        unsigned char B = (RGB & 0b00000011);
        unsigned char intensity = floorDivide(R+G+B+B, 3);
        RGB = 0;
        RGB += intensity << 5;
        RGB += intensity << 2;
        RGB += intensity >> 1;
        *(image+i) = RGB;
    }
}

int floorSqrt(int x)
{
    // Base cases
    if (x == 0 || x == 1)
    return x;

    // Staring from 1, try all numbers until
    // i*i is greater than or equal to x.
    int i = 1, result = 1;
    while (result <= x)
    {
      i++;
      result = multiply(i, i);
    }
    return i - 1;
}
