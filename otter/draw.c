#include "draw.h"

void draw_dot(unsigned int offset, RGB_332_type RGB, pixel_buf_type *pix_buf)
{
    /*
    pix_buf->bfr[pix_buf->cap] = RGB;
    // write four pixels at a time
    if (++pix_buf->cap == 4)
    {
        ((word *)VGA_ADDR)[offset] =
            (pix_buf->bfr[0] << 24) +
            (pix_buf->bfr[3] << 16) +
            (pix_buf->bfr[1] << 0);
        pix_buf->cap = 0;
    }
    */
    
    ((RGB_332_type *)VGA_ADDR)[offset] = RGB;
}

// draws an image starting at the adress 'image'
void draw_image(RGB_332_type *image)
{
    int offset = 0;
    pixel_buf_type pix_buf;

    for (int row = 0; row < Y_RES; row++)
    {
        for (int col = 0; col < X_RES; col++)
        {
            // get color data at that offset
            RGB_332_type RGB = *(image + offset);
            // draw it
            draw_dot(offset, RGB, &pix_buf);
            offset++;
        }
    }
}
