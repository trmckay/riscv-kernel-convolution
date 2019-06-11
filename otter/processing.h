#ifndef processing
#define processing

void grayscale(unsigned char *image, int len, int overwrite);

void shiftColor(unsigned char *image, int dim, int deltaR, int deltaG, int deltaB);

void convolve(unsigned char *image, int dim, int kernel[3][3], int divisor);

void sobel(unsigned char *image, int dim, int threshold);

#endif
