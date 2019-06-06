#ifndef processing
#define processing

void convolve(unsigned char *image, int len, int cols, int *kernel, unsigned char *dest, int divisor);

void grayscale(unsigned char *image);

int floorSqrt(int x);

#endif
