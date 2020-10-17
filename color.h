#pragma once

#include <stdint.h>

struct Color
{
    struct RGB {
        RGB(uint8_t r, uint8_t g, uint8_t b)
            :r(r), g(g), b(b)
        {
        }

        uint32_t to_8bit() const
        {
            return static_cast<uint8_t>(r * 7 / 255.0) << 5 | static_cast<uint8_t>(g * 7 / 255.0) << 2 | static_cast<uint8_t>(b * 3 / 255.0);
        }

        static RGB of_8bit(uint8_t rgb_8bit)
        {
            uint8_t r = (rgb_8bit >> 5) * 255 / 7;
            uint8_t g = ((rgb_8bit >> 2) & 0b111) * 255 / 7;
            uint8_t b = (rgb_8bit & 0b11) * 255 / 3;
            return RGB(r, g, b);
        }

        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
    };


    static uint16_t rgb_8bit_to_color565(uint8_t rgb_8bit)
    {
        RGB color = RGB::of_8bit(rgb_8bit);
        return GO.lcd.color565(color.r, color.g, color.b);
    }
};
