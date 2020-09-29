#pragma once

#include <odroid_go.h>
#include "image.h"

#include <map>
#include <utility>

struct Screen
{
    Screen(const Image& bg)
        : bg(bg)
    {
    }

    static uint32_t rgb_8bit_to_color565(uint8_t rgb_8bit)
    {
        uint8_t r = (rgb_8bit >> 5) * 255 / 7;
        uint8_t g = ((rgb_8bit >> 2) & 0b111) * 255 / 7;
        uint8_t b = (rgb_8bit & 0b11) * 255 / 3;
        return GO.lcd.color565(r, g, b);
    }

    void init()
    {
        for (size_t x = 0; x < bg.width; x++)
        {
            for (size_t y = 0; y < bg.height; y++)
            {
                uint32_t color = rgb_8bit_to_color565(bg.get_pixel(x, y));
                GO.lcd.drawPixel(x, y, color);
            }
        }
    }

    void set_pixel(int32_t x, int32_t y, uint8_t color)
    {
        if (x >= 0 && y >= 0 && x < bg.width && y < bg.height)
        {
            z_buff[{x, y}] = color;
        }
    }

    void render()
    {
        for (auto& it : z_buff)
        {
            auto key = it.first;
            uint16_t x = key.first;
            uint16_t y = key.second;
            uint32_t color = rgb_8bit_to_color565(it.second);
            GO.lcd.drawPixel(x, y, color);
        }
        z_buff.clear();
    }

    std::map <std::pair<uint16_t, uint8_t>, uint32_t> z_buff;
    const Image& bg;
};