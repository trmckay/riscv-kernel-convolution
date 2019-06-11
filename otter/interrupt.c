#include "draw.h"
#include "processing.h"

void interrupt(unsigned char *image, int dim)
{
    // addresses for MMIO
    unsigned int *LEDS = (unsigned int *) 0x11080000;
    unsigned int *SWITCHES = (unsigned int *) 0x11000000;

    // right eight switches for mode select
    int mode = *SWITCHES & 0b11111111;

    // integer values as read from switches to be used later
    // swA_B = SWITCHES[A:B] >> (B-1);
    int sw16_14 = (*SWITCHES & 0b1110000000000000) >> 13;
    int sw13_11 = (*SWITCHES & 0b0001110000000000) >> 10;
    int sw10_9 = (*SWITCHES & 0b0000001100000000) >> 8;
    int sw16_9 = (*SWITCHES & 0b1111111100000000) >> 8;

    /* kernels that can be used with the convolution algorithm
       of course, any arbitrary kernel can be used */
    int averageBlur[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
    int sharpen_1[3][3] = { {-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1} };
    int gaussian[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
    int edges[3][3] = { {-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1} };
    int sharpen_2[3][3] = { {0, -1, 0}, {-1, 5, -1}, {0, -1, 0} };

    /* right eight switches are read as an unsigned binary integer
       the mode is selected from this
       0000 - grayscale
       0001 - add color
       0010 - subtract color
       0011 - average blur
       0100 - sharpening (1)
       0101 - gaussian blur
       0110 - naive edge detection
       0111 - sharpening (2)
       1000 - sobel edge detection */
    switch (mode)
    {
        case 0:
            grayscale(image, dim, 0);
            break;
        case 1:
            // use the left 8 switches to control color channels
            shiftColor(image, dim, sw16_14, sw13_11, sw10_9);
            break;
        case 2:
            // use the left 8 switches to control color channels
            shiftColor(image, dim, -sw16_14, -sw13_11, -sw10_9);
            break;
        case 3:
            convolve(image, dim, averageBlur, 9);
            break;
        case 4:
            convolve(image, dim, sharpen_1, 1);
            break;
        case 5:
            convolve(image, dim, gaussian, 16);
            break;
        case 6:
            convolve(image, dim, edges, 1);
            break;
        case 7:
            convolve(image, dim, sharpen_2, 1);
            break;
        case 8:
            // use the left 8 switches to control threshold
            sobel(image, dim, sw16_9);
            break;
    }
}
