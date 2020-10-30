#pragma once

#include <stdint.h>

struct Image
{
    const uint8_t* data;
    const uint16_t width;
    const uint16_t height;

    uint8_t get_pixel(uint16_t x, uint16_t y) const
    {
        const uint32_t offset = x + y * width;
        return data[offset];
    }
};
