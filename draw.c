void drawDot(int col, int row, unsigned char RGB)
{
    volatile unsigned int *vgaAddr   = (volatile unsigned int *)  0x11100000;
    volatile unsigned char *vgaColor = (volatile unsigned char *) 0x11140000;

    int address = col & 0b111111111;
    int temp    = row & 0b011111111;
    temp = temp << 9;
    address = address | temp;
    *(vgaAddr) = address;
    *(vgaColor) = RGB;
}

void drawImage320x240(unsigned char *image)
{
    unsigned int len = 76800;
    unsigned int num_rows = 240;
    unsigned int num_cols = 320;

    // for each pixel
    int offset = 0;
    for (int row = 0; row < num_rows; row++)
    {
        for (int col = 0; col < num_cols; col++)
        {
            // draw it
            offset++;
            unsigned char RGB = *(image + offset);
            drawDot(col, row, RGB);
        }
    }
}
