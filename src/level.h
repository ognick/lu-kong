#pragma once

#include "color.h"
#include "point.h"
#include "res/image.h"
#include "screen.h"
#include "direction.h"

struct Level
{
                            // NONE UP DOWN LEF RIGHT
    const int8_t offset_x[5] = {0, 0, 0, -1, 1};
    const int8_t offset_y[5] = {0, -1, 1, 0, 0};

    enum class SurfaceType : uint8_t
    {
        BLOCK = Color::BLACK_8BIT,
        STAIR = Color::RED_8BIT,
        FREE // OTHER
    };

    struct BoundingBox
    {
        int16_t x = 0;
        int16_t y = 0;
        uint8_t width = 0;
        uint8_t height = 0;
    };

    Level(const Image& map, Screen& screen, uint8_t gravity = 1)
        : map(map)
        , screen(screen)
        , gravity(gravity)
    {
    }

    SurfaceType get_surface_type(uint16_t x, uint16_t y) const
    {
        SurfaceType type = static_cast<SurfaceType>(map.get_pixel(x, y));
        if (type == SurfaceType::BLOCK)
            return SurfaceType::BLOCK;
        if (type == SurfaceType::STAIR)
            return SurfaceType::STAIR;
        return SurfaceType::FREE;
    }

    SurfaceType move_box_one_step(BoundingBox& bb, Direction direction)
    {
        for (uint8_t step=0; step < gravity; ++step)
        {
            bool fall_down = true;
            for (uint8_t offset_x = 0; offset_x < bb.width; ++offset_x)
            {
                const SurfaceType down_type = get_surface_type(bb.x + offset_x, bb.y + bb.height);
                fall_down &= (down_type == SurfaceType::FREE);
            }

            if (fall_down)
            {
                ++bb.y;
            }
        }

        const bool is_vertical = (direction == Direction::UP || direction == Direction::DOWN);
        const uint8_t offset_index = static_cast<uint8_t>(direction);
        const int16_t move_x = offset_x[offset_index];
        const int16_t move_y = offset_y[offset_index];
        bool is_stair = false;
        bool can_move = true;

        for (int16_t offset_x = 1; can_move && offset_x < bb.width; ++offset_x)
        {
            for (int16_t offset_y = 1; offset_y < bb.height; ++offset_y)
            {
                const int16_t curr_x = bb.x + offset_x;
                const int16_t curr_y = bb.y + offset_y;
                const int16_t next_x = move_x + curr_x;
                const int16_t next_y = move_y + curr_y;
                const SurfaceType curr_type = get_surface_type(curr_x, curr_y);
                const SurfaceType next_type = get_surface_type(next_x, next_y);
                is_stair |= (curr_type == SurfaceType::STAIR || next_type == SurfaceType::STAIR);
                if (next_type == SurfaceType::BLOCK)
                {
                    can_move = false;
                    break;
                }
            }
        }

        if (is_vertical && !is_stair)
        {
            can_move = false;
        }

        if (can_move)
        {
            bb.x += move_x;
            bb.y += move_y;
        }

        if (is_stair)
        {
            return SurfaceType::STAIR;
        }

        return SurfaceType::FREE;
    }

    SurfaceType move_object(BoundingBox& bb, Direction direction, uint8_t speed)
    {
        SurfaceType surface_type = SurfaceType::STAIR;
        for (uint8_t step = 0; step < speed; ++step)
        {
            SurfaceType curr_surface_type = move_box_one_step(bb, direction);
            if (curr_surface_type == SurfaceType::FREE)
            {
                surface_type = SurfaceType::FREE;
            }

        }

        return surface_type;
    }

    const Image& map;
    Screen& screen;
    uint8_t gravity;
};
