#include "draw.h"
#include "processing.h"

void interrupt(unsigned char *image, int dim)
{
    unsigned int *LEDS = (unsigned int *) 0x11080000;
    unsigned int *SWITCHES = (unsigned int *) 0x11000000;

    int mode = *SWITCHES & 0b11111111;

    // integer values as read from switches to be used later
    int sw16_14 = (*SWITCHES & 0b1110000000000000) >> 13;
    int sw13_11 = (*SWITCHES & 0b0001110000000000) >> 10;
    int sw10_9 = (*SWITCHES & 0b0000001100000000) >> 8;
    int sw16_9 = (*SWITCHES & 0b1111111100000000) >> 8;

    int threshold = *(SWITCHES) & 0b1111111100000000;

    int averageBlur[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
    int sharpen_1[3][3] = { {-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1} };
    int gaussian[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
    int edges[3][3] = { {-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1} };
    int sharpen_2[3][3] = { {0, -1, 0}, {-1, 5, -1}, {0, -1, 0} };

    switch (mode)
    {
        case 0:
            grayscale(image, dim, 0);
            break;
        case 1:
            shiftColor(image, dim, sw16_14, sw13_11, sw10_9);
            break;
        case 2:
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
            sobel(image, dim, sw16_9);
            break;
    }
}
