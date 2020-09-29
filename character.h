#pragma once

#include "animation.h"
#include <map>

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

enum class Direction : uint8_t
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Character
{
    Character(Screen& screen)
        : screen(screen)
    {
    }

    void add_animation(AnimationType type, const Image& img, uint8_t frame_count)
    {
        animations.emplace(type, Animation{screen, img, frame_count});
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
            Animation& animation = find_it->second;
            animation.clear();
        }
    }

    void draw(int16_t x, int16_t y)
    {
        auto find_it = animations.find(type);
        if (find_it != animations.end())
        {
            Animation& animation = find_it->second;
            const uint8_t sprite_idx = frame_idx % animation.horizontal_frame_count;
            const bool reverse = (dir == Direction::LEFT);
            animation.draw(x, y, sprite_idx, reverse);
        }
        ++frame_idx;
    }

    Screen& screen;
    std::map<AnimationType, Animation> animations;
    AnimationType type = AnimationType::IDLE;
    Direction dir = Direction::NONE;
    uint8_t frame_idx = 0;
};
