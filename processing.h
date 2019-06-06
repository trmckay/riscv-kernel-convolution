#ifndef processing
#define processing

void convolve(unsigned char *image, int len, int cols, int *kernel, unsigned char *dest, int divisor);

void grayscale(unsigned char *image, unsigned char *end);

int floorSqrt(int x);

int multiply(int a, int b);

int floorDivide(int dividend, int divisor);

#endif
