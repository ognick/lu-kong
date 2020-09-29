#pragma once

struct Image
{
    const unsigned char* data;
    const unsigned short width;
    const unsigned short height;

    unsigned char get_pixel(unsigned short x, unsigned short y) const
    {
        const unsigned long offset = x + y * width;
        return data[offset];
    }
};
