/* all pixel values in memory <- grayscale equivalent
   used registers: a0, a1, a2 (color values, not saved),
                   t0 (current address, not saved), t1 (RGB value, not saved),
                   t2 (bit-shifted intensity, not saved)
arguments:
a0: start address of image
a1: end of image
*/
void grayscale(unsigned char *image, unsigned char *end)
{
    // for each pixel
    for (int i = 0; i < (end-image); i++)
    {
        // isolate R, G, and B values
        unsigned char RGB = image[i];
        unsigned char R = (RGB & 0b11100000) >> 5;
        unsigned char G = (RGB & 0b00011100) >> 2;
        unsigned char B = (RGB & 0b00000011);
        unsigned char intensity = (red+green+blue+blue)/3;
        image[i] = 0;
        image[i] += intensity << 5;
        image[i] += intensity << 2;
        image[i] += intensity >> 1;
    }
}
