#pragma once

#include "color.h"
#include "point.h"
#include "image.h"
#include "direction.h"

#include<Arduino.h>
struct Level
{
                            // NONE UP DOWN LEF RIGHT
    const int8_t offset_x[5] = {0, 0, 0, -1, 1};
    const int8_t offset_y[5] = {0, -1, 1, 0, 0};
    const uint8_t gravity_speed = 10;

    enum SurfaceType : uint8_t
    {
        BLOCK = 0x00, // BLACK
        STAIR = 0xe0, // RED
        FREE // OTHER
    };

    struct SurfacePoint
    {
        SurfacePoint(int16_t x, int16_t y, SurfaceType type)
            : x(x), y(y), type(type)
        {
        }

        int16_t x;
        int16_t y;
        SurfaceType type;
    };

    struct BoundingBox
    {
        int16_t x;
        int16_t y;
        uint8_t width;
        uint8_t height;
    };

    Level(const Image& map, uint8_t gravity = 0)
        : map(map)
        , gravity(gravity)
    {
    }

    SurfaceType get_surface_type(uint16_t x, uint16_t y) const
    {
        uint8_t color = map.get_pixel(x, y);
        if (color == SurfaceType::BLOCK)
            return SurfaceType::BLOCK;
        if (color == SurfaceType::STAIR)
            return SurfaceType::STAIR;
        return SurfaceType::FREE;
    }


    SurfacePoint get_next_character_point(int16_t x, int16_t y, Direction direction, uint8_t speed)
    {
        const uint8_t offset_index = static_cast<uint8_t>(direction);
        const int8_t move_x = offset_x[offset_index];
        const int8_t move_y = offset_y[offset_index];
        const SurfaceType curr_surface_type = get_surface_type(x, y);
        SurfacePoint next_point(x, y, curr_surface_type);
        const bool is_vertical = (direction == Direction::UP || direction == Direction::DOWN);
        for (uint8_t step = 0; step < speed; ++ step) {
            const int16_t next_x = move_x * step + x;
            int16_t next_y = move_y * step + y;
            for (uint8_t i=0; i < gravity_speed; ++i)
            {
                if (get_surface_type(next_x, next_y + 1) != SurfaceType::FREE) // Gravity
                {
                    break;
                }

                ++next_y;
            }

            const SurfaceType next_surface_type = get_surface_type(next_x, next_y);
            if (next_surface_type == BLOCK)
            {
                break;
            }

            if (is_vertical && curr_surface_type != SurfaceType::STAIR)
            {
                break;
            }

            next_point.x = next_x;
            next_point.y = next_y;
            next_point.type = next_surface_type;
        }

        return next_point;

//        char str[80];
//        sprintf(str, "%d [%d x %d] [%l] -> [%d x %d] [%l]", offset_index, x, y, curr_surface_type, next_x, next_y, curr_surface_type);
//        Serial.println(str);

    }

    const Image& map;
    uint8_t gravity;
};
