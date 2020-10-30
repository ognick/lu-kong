#pragma once

#include "sprite_animation.h"
#include "direction.h"
#include <map>
#include <stdint.h>

enum class AnimationType : uint8_t
{
    IDLE,
    RUN,
    WALK,
    ATTACK,
    DEATH,
    HURT,
    CLIMB
};

struct CharacterAnimation
{
    CharacterAnimation(Screen& screen)
        : screen(screen)
    {
    }

    void set_hot_point(uint8_t x, uint8_t y)
    {
        hot_point_x = x;
        hot_point_y = y;
    }

    void add_animation(AnimationType type, const Image& img, uint8_t frame_count)
    {
        animations.emplace(type, SpriteAnimation{screen, img, frame_count});
    }

    void set_animation_type(AnimationType type_)
    {
        if (type != type_)
        {
            clear();
            type = type_;
            frame_idx = 0;
        }
    }
    void set_direction(Direction dir_)
    {
        if (dir != dir_)
        {
            clear();
            dir = dir_;
            frame_idx = 0;
        }
    }

    void clear()
    {
        auto find_it = animations.find(type);
        if (find_it != animations.end())
        {
            SpriteAnimation& animation = find_it->second;
            animation.clear();
        }
    }

    void draw(int16_t x, int16_t y, bool pause = false)
    {
        auto find_it = animations.find(type);
        if (find_it != animations.end())
        {
            SpriteAnimation& animation = find_it->second;
            const uint8_t sprite_idx = frame_idx % animation.horizontal_frame_count;
            const bool reverse = (dir == Direction::LEFT);
            animation.draw(x - hot_point_x, y - hot_point_y, sprite_idx, reverse);
        }

        if (!pause)
        {
            ++frame_idx;
        }
    }

    Screen& screen;
    std::map<AnimationType, SpriteAnimation> animations;
    AnimationType type = AnimationType::IDLE;
    Direction dir = Direction::NONE;
    uint8_t frame_idx = 0;

    uint8_t hot_point_x = 0;
    uint8_t hot_point_y = 0;
};
