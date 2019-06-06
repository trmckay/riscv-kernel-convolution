#include "draw.h"
#include "processing.h"

void interrupt(unsigned char *image)
{
    volatile int *LEDS = (volatile int *)0x11080000;
    *LEDS = 0b1010101010101010;
    grayscale(image);
    drawImage320x240(image);
}
