#ifndef VGA_H
#define VGA_H

typedef unsigned char RGB_332_type;
typedef unsigned int coord_type;
typedef int word;

typedef struct pb {
    int bfr[4];
    int cap;
} pixel_buf_type;

#define X_RES (320)
#define Y_RES (240)
#define VGA_ADDR (0x12000000)

#endif
