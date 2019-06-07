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
