#include "draw.h"
#include "processing.h"

#ifndef MULTICYCLE
    void draw_dot(coord_type offset, RGB_332_type RGB)
    {
        ((RGB_332_type *)VGA_ADDR)[offset] = RGB;
    }
#endif

#ifdef MULTICYCLE
    void draw_dot(int col, int row, unsigned char RGB)
    {
        unsigned int *vgaAddr   = (unsigned int *)0x11100000;
        unsigned char *vgaColor = (unsigned char *)0x11140000;

        int address = col & 0b111111111;
        int temp    = row & 0b011111111;
        temp = temp << 9;
        address = address | temp;
        *(vgaAddr)  = address;
        *(vgaColor) = RGB;
    }
#endif

// draws an image starting at the adress 'image'
void draw_image(RGB_332_type *image)
{
    int offset = 0;
    for (int row = 0; row < Y_RES; row++)
    {
        for (int col = 0; col < X_RES; col++)
        {
            RGB_332_type RGB = image[offset];
            DRAW_DOT(RGB);
            offset++;
        }
    }
}

void sobel_bench(RGB_332_type *image)
{
    int sw_val = 1;
    *((int *)LEDS_ADDR) = 0;

    while (sw_val != 0)
        sw_val = *((int *)SWITCHES_ADDR);

    *((int *)LEDS_ADDR) = 1;
    sobel(image, 0);
    *((int *)LEDS_ADDR) = 0;

    while (sw_val == 0)
        sw_val = *((int *)SWITCHES_ADDR);
}

void run_img_proc(RGB_332_type *image)
{
    draw_image(image);
    #ifdef BENCH
        sobel_bench(image);
    #endif
}
