#include <stdio.h>
#include <stdlib.h>

/*
IMPORTANT!
Most if not all image processing functions assume a SQUARE image.
This is not a fundamental limitation of the algorithms,
but a technique to save time and memory.
It is costly to calculate the length of a rectangular image due to the lack of
hardware multiplication on the OTTER. Therefore, many of the dimensions are
assumed (either hardcoded as 160x160 or any square).
*/

/* all pixel values in memory <- grayscale equivalent
arguments:
a0: start address of image
a1: end of image
*/
void grayscale(unsigned char *image, int len)
{
    // for each pixel
    // (end - image) = length of array
    for (int i = 0; i < len; i++)
    {
        // isolate R, G, and B values
        unsigned char RGB = *(image+i);
        unsigned char R = (RGB & 0b11100000) >> 5;
        unsigned char G = (RGB & 0b00011100) >> 2;
        unsigned char B = (RGB & 0b00000011);
        unsigned char intensity = (R+G+B+B)/3;
        RGB = 0;
        RGB += intensity << 5;
        RGB += intensity << 2;
        RGB += intensity >> 1;
        *(image+i) = RGB;
    }
}

/* simplified "good enough" alternative to vector magnitude to save some time
 * basically just an average, but left as a function to test alternatives */
unsigned char _magnitude(unsigned char i, unsigned char j)
{
    return (i + j) << 1;
}

/* algorithm for kernel convolution:
 * [ a, b, c ]   [ r, s, t ]
 * | d, e, f | * | u, v, w | = (a*z) + (b*y) + (c*x) + (d*w) ... + (j*r)
 * [ h, i, j ]   [ x, y, z ]
*/
unsigned char _applyKernel(unsigned char *pixel, int dim, char kernel[9])
{
    unsigned char pixels[9] = {
        *(pixel - dim - 1),
        *(pixel - dim),
        *(pixel - dim + 1),
        *(pixel - 1),
        *(pixel),
        *(pixel + 1),
        *(pixel + dim - 1),
        *(pixel + dim),
        *(pixel + dim + 1)
    };
    int convolution = 0;
    for (int i = 0; i < 9; i++) {
        convolution += pixels[i] * kernel[8-i];
    }
    // shifted right to fit into byte
    convolution = (unsigned char)(abs(convolution)>>3);
    return convolution;
}

void sobel(unsigned char *image, int dim, int len)
{
    char Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    char Gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    unsigned char *GxConvs = (unsigned char *)(image + len);
    unsigned char *GyConvs = (unsigned char *)(GxConvs + dim + 2);

    for (int offset = 0; offset < len; offset++)
    {
        if (offset > 7) {
            // write magnitude of <dx[dim+1], dy[dim+1]> to mem[image + offset - dim - 2]
            *(image + offset - dim - 2) = _magnitude(*(GxConvs + dim + 1), *(GyConvs + dim + 1));
        }
        // shift contents of dx and dy right one index
        for (int i = dim + 2; i > 0; i--) {
            *(GxConvs + i) = *(GxConvs + i - 1);
            *(GyConvs + i) = *(GyConvs + i - 1);
        }
        // insert Gx(image+offset) into dx[0] and same for Gy
        *GxConvs = _applyKernel((image+offset), dim, Gx);
        *GyConvs = _applyKernel((image+offset), dim, Gy);
    }
    // write remaining contents of convolutions arrays
    for (int i = 0; i < dim + 2; i++) {
        *(image - (dim + 2) + i) = _magnitude(*(GxConvs + i), *(GyConvs + i));
    }
}

void generalConvolve(unsigned char *image, int dim, int len, char kernel[9])
{
    /* initialize a pointer to an array that will store intermediate convolution results
     * the array is located directly after the pixel array */
    unsigned char *convs = (unsigned char *)(image + len);

    for (int offset = 0; offset < len; offset++)
    {
        /* after a certain point, a pixel will no longer be used for future convolutions
         * this point is when the loop has passed a pixel by one row length + 2
         * (image + offset) is the current pixel, so
         * image + offset - (dim + 2) is a pointer to the pixel that can be overwritten
         * its overwritten with the last item in the convolutions array */
        if (offset > 7) {
            *(image + offset - (dim + 2)) = *(convs + dim + 1);
        }
        /* next all elements in the array are shifted right on index, leaving the first index open
        this is where the new convolution is placed */
        for (int i = dim + 2; i > 0; i--) {
            *(convs + i) = *(convs + i - 1);
        }
        *convs = _applyKernel((image+offset), dim, kernel);
    }
    // there are still elements in the intermediate elements in the array
    for (int i = 0; i < dim + 2; i++) {
        *(image - (dim + 2) + i) = *(convs + i);
    }
}
