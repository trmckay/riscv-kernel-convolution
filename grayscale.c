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
        unsigned char intensity = (R+G+B+B)/3;
        RGB = 0;
        RGB += intensity << 5;
        RGB += intensity << 2;
        RGB += intensity >> 1;
        *(image+i) = RGB;
    }
}
