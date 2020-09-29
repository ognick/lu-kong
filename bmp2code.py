#!/usr/bin/python3

'''
# Example:
> python bmp2code.py m5_logo.bmp
> Out:m5_logo_img.c Done!
'''

import os
import sys

from PIL import Image

in_name = sys.argv[1]
[name, ext] = in_name.split('.')
out_name = name + '_img.cpp'
file_size = os.path.getsize(in_name)

img = Image.open(in_name)
(width, height) = img.size
data = img.load()
new_image = Image.new('RGB', img.size, 'white')
new_data = new_image.load()


def rgb24_to_8b(rgb_24bit):
    (r, g, b) = rgb_24bit
    return int(r * 7 / 255) << 5 | int(g * 7 / 255) << 2 | int(b * 3 / 255)


def decode_rgb(rgb_8bit):
    r = rgb_8bit >> 5
    g = (rgb_8bit >> 2) & 0b111
    b = rgb_8bit & 0b1
    return r, g, b


def rgb8_to_24(rgb_8bit):
    (r, g, b) = decode_rgb(rgb_8bit)
    r = int(r * 255 / 7)
    g = int(g * 255 / 7)
    b = int(b * 255 / 3)
    return r, g, b


def write_line(f, line=''):
    f.write((line + '\n').encode('utf-8'))


with open(out_name, 'wb+') as outfile:
    write_line(outfile, '#include "image.h"')
    write_line(outfile, f'const unsigned char {name}_data [{width * height}] = ' + '{')
    for y in range(height):
        for x in range(width):
            color = data[x, y]
            if not isinstance(color, int):
                rgb = color[0:3]
                color = rgb24_to_8b(rgb)
            d = "0x%02x," % color
            outfile.write(d.encode('utf-8'))
            new_data[x, y] = rgb8_to_24(color)
        write_line(outfile)
    write_line(outfile, '\n};')
    constructor_args = '{' + f'{name}_data, {width}, {height}' + '}'
    write_line(outfile, f'extern const Image {name}_img{constructor_args};')

new_image.show()
print('Out:' + out_name + ' Done!')
