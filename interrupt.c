#include "draw.h"
#include "processing.h"

void interrupt(unsigned char *image)
{
    int *LEDS = (int *)0x11080000;
    *LEDS = 0b1010101010101010;
    grayscale(image, 6400);
    drawImage(image, 80);
}
