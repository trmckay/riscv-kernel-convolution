#ifndef processing
#define processing

void convolve(unsigned char *image, int len, int cols, int *kernel, unsigned char *dest, int divisor);

void grayscale(unsigned char *image, int len);

int floorSqrt(int x);

unsigned char sobelConvolve(unsigned char *pixel, int dim, char kernel[9]);

void generalConvolve(unsigned char *image, int dim, int len, char kernel[9]);

#endif
