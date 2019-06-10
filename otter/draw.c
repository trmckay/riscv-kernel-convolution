#include <stdio.h>

void drawDot(int col, int row, unsigned char RGB)
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

void drawImage(unsigned char *image, int dim)
{
    // for each pixel
    int offset = 0;
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            unsigned char RGB = *(image + offset);
            drawDot(col, row, RGB);
            offset++;
        }
    }
}

/*
typedef struct point point;
struct point {
   int x;
   int y;
};

void drawCoords(point coordinates[], unsigned int numPts, unsigned char RGB)
{
    for (int i = 0; i < numPts; i++)
    {
        point p = coordinates[i];
        drawDot(p.x, p.y, RGB);
    }
}

void splashScreen(int shiftX, int shiftY)
{
    unsigned char orange = 0b11101100;
    unsigned char blue = 0b00110011;

    point O[14] = {
        {0+shiftX, 1+shiftY}, {0+shiftX, 2+shiftY}, {0+shiftX, 3+shiftY},
        {1+shiftX, 0+shiftY}, {1+shiftX, 4+shiftY}, {2+shiftX, 0+shiftY},
        {2+shiftX, 4+shiftY}, {3+shiftX, 0+shiftY}, {3+shiftX, 4+shiftY},
        {4+shiftX, 0+shiftY}, {4+shiftX, 4+shiftY}, {5+shiftX, 1+shiftY},
        {5+shiftX, 2+shiftY}, {5+shiftX, 3+shiftY}
    };
    point V[9] = {
        {4+shiftX, 6+shiftY}, {4+shiftX, 7+shiftY},  {5+shiftX, 8+shiftY},
        {5+shiftX, 9+shiftY}, {6+shiftX, 10+shiftY}, {7+shiftX, 9+shiftY},
        {7+shiftX, 8+shiftY}, {8+shiftX, 7+shiftY},  {8+shiftX, 6+shiftY}
    };

    drawCoords(O, 14, orange);
    drawCoords(V, 9, blue);
}
*/
