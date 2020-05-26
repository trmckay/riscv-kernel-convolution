#ifndef PROCESSING_H
    #define PROCESSING_H

    #include "draw.h"
    #include "otter.h"

    void grayscale(RGB_332_type *image, int overwrite);
    void shift_color(RGB_332_type *image, int d_rd, int d_gr, int d_bl);
    void convolve(RGB_332_type *image, int kernel[3][3], int divisor);
    void sobel(RGB_332_type *image, int threshold);

#endif
