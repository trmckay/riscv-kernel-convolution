#include "draw.h"
#include "processing.h"

void interrupt(unsigned char *image)
{
    grayscale(image, 40000);
    drawImage(image, 200);
}
