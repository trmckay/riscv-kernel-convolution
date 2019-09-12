This simple Python script takes a .png, .jpg, or .jpeg (more may work but only these are tested) and coverts it to a byte array for RISC-V assembly.

Each byte represents a RGB value where the 3 most significant bits are red, the 3 following bits are green, and the 2 least significant bits are blue (it's important to note that blue is only 2 bits as opposed to three; this should be taken into account when doing any sort of image processing).

The pixels are in order from left to right.
(i.e.):
1  2  3  4
5  6  7  8
9  10 11 12

Usage:
    python img_script.py my_image.png <-f>

Requirements:
    Python Image Library or Pillow
    You can get Pillow with 'pip install Pillow'
