#ifndef DRAW_H
    #define DRAW_H

    #include "otter.h"

    void draw_image(RGB_332_type *image);
    void sobel_bench(RGB_332_type *image);

    #ifndef MULTICYCLE
        void draw_dot(unsigned int offset, unsigned char RGB);
        #define DRAW_DOT(RGB) draw_dot(offset, RGB)
    #endif

    #ifdef MULTICYCLE
        void draw_dot(int col, int row, unsigned char RGB);
        #define DRAW_DOT(RGB) draw_dot(col, row, RGB)
    #endif

#endif
