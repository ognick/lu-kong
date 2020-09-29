#pragma once

#include "screen.h"

struct Animation
{
    Animation(Screen& screen, const Image& img, uint8_t horizontal_frame_count)
            : screen(screen)
            , img(img)
            , horizontal_frame_count(horizontal_frame_count)
    {
    }

    void fill(int16_t x0, int16_t y0, uint8_t horizontal_frame_idx, bool reverse, bool clear)
    {
        const uint8_t transparent_color = img.get_pixel(0, 0);
        const size_t frame_width = img.width / horizontal_frame_count;
        const size_t offset_x = frame_width * horizontal_frame_idx;
        for (size_t x = 0; x < frame_width; x++)
        {
            for (size_t y = 0; y < img.height; y++)
            {
                int16_t img_x = x;
                if (reverse) {
                    img_x = frame_width - x - 1;
                }
                uint8_t color = img.get_pixel(img_x + offset_x, y);
                if (color != transparent_color)
                {
                    if (clear)
                    {
                        color = screen.bg.get_pixel(x0 + x, y0 + y);
                    }
                    screen.set_pixel(x0 + x, y0 + y, color);
                }
            }
        }
    }

    void clear()
    {
        if (__is_init)
        {
            fill(__x, __y, __horizontal_frame_idx, __reverse, true);
        }
    }

    void draw(int16_t x, int16_t y, uint8_t horizontal_frame_idx, bool reverse)
    {
        clear();

        __is_init = true;
        __horizontal_frame_idx = horizontal_frame_idx;
        __x = x;
        __y = y;
        __reverse = reverse;
        fill(__x, __y, __horizontal_frame_idx, __reverse, false);
    }

    const uint8_t horizontal_frame_count;
    uint8_t __horizontal_frame_idx;
    int16_t __x;
    int16_t __y;
    bool __reverse;
    bool __is_init = false;
    Screen& screen;
    const Image& img;
};
