// image is the beginning of the image array
// matrix is pointer to beginning of a 3x3 kernel matrix
void convolve(unsigned char *image, int rows, int cols, int *matrix, int *dest, int divisor)
{
    int len = rows * cols;
    // for each pixel in the array
    for (int i = 0; i < len; i++)
    {
        // collect its neighbors in an array
        unsigned char pixelMatrix[] = buildMatrix(image+i);
        int sum = 0;
        // apply the kernel
        /*
        pixel and kernel matrices in relation to the j value
        */
        for (int j = 0, j < 9, j++)
        {
            sum += pixelMatrix[i] * pixelMatrix[9-i];
        }
        // save the result (sum) in memory
        if (divisor != 0 && divisor != 1) sum = sum/divisor;
        dest[i] = sum;
    }
}

/*
returns neighbors of pixel as:
[a, b, c, d, e, f, g]
where e is the current pixel and neighbors are arranged as:
-------------------
 a       b       c
 d       e       d
 e       f       g
-------------------
*/
void buildMatrix(unsigned char pixel*)
{
    unsigned char *neighbors[9] = {
        pixel - cols - 1,
        pixel - cols,
        pixel - cols + 1,
        pixel - 1,
        pixel,
        pixel + 1,
        pixel + cols - 1,
        pixel + cols + 1
    };
    return neighbors;
}
