#ifndef processing
#define processing

void grayscale(unsigned char *image, int len, int overwrite);

// void sobel(unsigned char *image, int dim);

void shiftColor(unsigned char *image, int dim, int deltaR, int deltaG, int deltaB);

void averageBlur(unsigned char *image, int dim);

#endif
