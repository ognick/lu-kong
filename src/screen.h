#pragma once

#include <odroid_go.h>
#include "color.h"
#include "res/image.h"

#include <stdint.h>
#include <map>
#include <utility>

struct Screen
{
    Screen(const Image& bg)
        : bg(bg)
    {
    }

    void init()
    {
        for (uint16_t x = 0; x < bg.width; x++)
        {
            for (uint16_t y = 0; y < bg.height; y++)
            {
                uint16_t color = Color::rgb_8bit_to_color565(bg.get_pixel(x, y));
                GO.lcd.drawPixel(x, y, color);
            }
        }
    }

    void clear_pixel(int32_t x, int32_t y)
    {
        set_pixel(x, y, bg.get_pixel(x, y));
    }

    void set_pixel(int32_t x, int32_t y, uint8_t color)
    {
        if (x >= 0 && y >= 0 && x < bg.width && y < bg.height)
        {
            z_buff[{x, y}] = color;
        }
    }

    bool is_override(int32_t x, int32_t y)
    {
        return (x >= 0 && y >= 0 && x < bg.width && y < bg.height && z_buff.find({x, y}) != z_buff.end());
    }

    void render()
    {
        for (auto& it : z_buff)
        {
            auto key = it.first;
            uint16_t x = key.first;
            uint16_t y = key.second;
            uint32_t color = Color::rgb_8bit_to_color565(it.second);
            GO.lcd.drawPixel(x, y, color);
        }
        z_buff.clear();
    }

    std::map <std::pair<uint16_t, uint8_t>, uint8_t> z_buff;
    const Image& bg;
};