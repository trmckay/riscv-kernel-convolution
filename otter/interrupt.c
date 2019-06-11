#include "draw.h"
#include "processing.h"

void interrupt(unsigned char *image)
{
    unsigned int *LEDS = (unsigned int *) 0x11080000;
    unsigned int *SWITCHES = (unsigned int *) 0x11000000;

    int deltaR = (*SWITCHES & 0b1110000000000000) >> 13;
    int deltaG = (*SWITCHES & 0b0001110000000000) >> 10;
    int deltaB = (*SWITCHES & 0b0000001100000000) >> 8;

    int mode = *SWITCHES & 0b11111111;

    int averageBlur[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
    int sharpen_1[3][3] = { {-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1} };
    int gaussian[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
    int edges[3][3] = { {-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1} };
    int sharpen_2[3][3] = { {0, -1, 0}, {-1, 5, -1}, {0, -1, 0} };

    switch (mode)
    {
        case 0:
            grayscale(image, 200, 0);
            break;
        case 1:
            shiftColor(image, 200, deltaR, deltaG, deltaB);
            break;
        case 2:
            shiftColor(image, 200, -deltaR, -deltaG, -deltaB);
            break;
        case 3:
            convolve(image, 200, averageBlur, 9);
            break;
        case 4:
            convolve(image, 200, sharpen_1, 1);
            break;
        case 5:
            convolve(image, 200, gaussian, 16);
            break;
        case 6:
            convolve(image, 200, edges, 1);
            break;
        case 7:
            convolve(image, 200, sharpen_2, 1);
            break;
    }
}
