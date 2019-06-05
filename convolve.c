/* image is the beginning of the image array
 * matrix is pointer to beginning of a 3x3 kernel matrix
 * convolves a kernel accross a image referenced by 'image'
 * saves the result of each convolution as an array of unsigned chars at 'dest'
 * ONLY WORKS ON GRAYSCALE IMAGES
 * convolutions with color require triple the memory, which is not available on the OTTER
 */
void convolve(unsigned char *image, int len, int cols, int *kernel[], unsigned char *dest, int divisor)
{
    // for each pixel in the array
    for (int i = 0; i < len; i++)
    {
        /*
         * collect pixel its neighbors in an array
         * image+i is a pointer to the current pixel
         * pixelMatrix = [a, b, c, d, e, f, g, h, i]
         * where e is the current pixel, *(image+i) and pixels are arranged as
         *  _______
         * | a b c |
         * | d e f |
         * | g h i |
         *  ‾‾‾‾‾‾‾
        */
        unsigned char *currPx = image + i;
        unsigned char pixelMatrix[9] = {
            *(currPx - cols - 1),
            *(currPx - cols),
            *(currPx - cols + 1),
            *(currPx - 1),
            *(currPx),
            *(currPx + 1),
            *(currPx + cols - 1),
            *(currPx + cols + 1)
        };
        unsigned char convolution = 0;
        int intensity;
        int kernalVal;
        // apply the kernel
        for (int j = 0; j < 9; j++)
        {
            // sample red value for intensity, this is arbitrary
            intensity = (int)((pixelMatrix[j] & 0b11100000) >> 5);
            kernalVal = *kernel[8-j];
            convolution += kernalVal * intensity;
        }
        // save the result (sum) in memory
        if (divisor != 0 && divisor != 1) convolution = convolution/divisor;
        *(dest+i) = convolution;
    }
}
