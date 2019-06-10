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

    switch (mode)
    {
        case 0:
            grayscale(image, 200, 0);
            break;
        case 1:
            shiftColor(image, 200, deltaR, deltaG, deltaB);
            break;
    }
}
