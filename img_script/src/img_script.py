import sys
import subprocess
from PIL import Image

extensions = ['.png', '.jpg', '.jpeg']

def image_to_bytes(img_name):
    # open image in read mode
    img = Image.open(img_name, 'r')
    # convert it to 24bit RGB
    img = img.convert(mode='RGB', colors=256)
    # scale it down to 320x240
    img = img.resize((200, 200))
    # get pixel data as a Python List
    pixels = list(img.getdata())
    # empty list containing new 8bit RGB data
    rgb8_values = []
    for pixel in pixels:
        # scale R value to 3 bits
        # bitshift left 5 times to but it in the most significant bits
        rgb = int((pixel[0]/255.0)*8) << 5
        # scale G value to 3 bits and bitshift
        rgb += int((pixel[1]/255.0)*8) << 2
        # scale B value to 2 bits and bitshift
        rgb += int((pixel[2]/255.0)*4)
        # add the 8bit value to the list of RGB data
        rgb8_values.append(min(rgb, 0xFF))
    # return the completed list
    return rgb8_values

def array_to_output(rgbs):
    # format output for RISC-V RV32I assembler680
    output = '.data\n\timg:    .byte '
    for rgb in rgbs:
        output += str(hex(rgb)) + ", "
    # write the output to 'data.txt' and open it
    with open('data.txt', 'w+') as f:
        print('opening file')
        f.write(str(output[:len(output)-2]))

def main(image_path):
    rgbs = image_to_bytes(image_path)
    array_to_output(rgbs)
    print("done!\nyour array can be found at \'./data.txt\'")
    exit()

if __name__ == '__main__':
    # check correct arg length
    if ((len(sys.argv) < 2) or (len(sys.argv) > 3)):
        print('Usage: \'python img_script.py <path to image> <-f>\'')
        exit()
    if ((len(sys.argv) == 3)):
        if (sys.argv[2] == '-f'):
            main(sys.argv[1])
        else:
            print('Argument \'' + sys.argv[2] + '\' ivalid.')
            print('Valid arguments: \'-f\'')
            exit()
    # check for valid file extension
    for ext in extensions:
        if (sys.argv[1].find(ext) > -1):
            main(sys.argv[1])
    print('Invalid image file, use \'-f\' to force')
