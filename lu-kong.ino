#include "image.h"
#include "screen.h"
#include "character_animation.h"
#include "level.h"

#include <odroid_go.h>
#include <stdint.h>

extern const Image player_walk_img;
extern const Image player_idle_img;
extern const Image player_climb_img;
extern const Image player_attack_img;
extern const Image bg_img;

Screen screen(bg_img);
CharacterAnimation player(screen);
Level level(bg_img);

void setup()
{
    GO.begin(9600);
    screen.init();
    player.add_animation(AnimationType::IDLE, player_idle_img, 4);
    player.add_animation(AnimationType::WALK, player_walk_img, 6);
    player.add_animation(AnimationType::CLIMB, player_climb_img, 6);
    player.add_animation(AnimationType::ATTACK, player_attack_img, 6);
    player.set_hot_point(24, 48);
}

const uint8_t fps = 7;
const uint32_t frame_time_ms = static_cast<uint32_t>(1 / static_cast<float>(fps) * 1000);
const uint16_t step = 6;
uint8_t attack_frame_count = 0;
int32_t pos_obj_y = 90;
int32_t pos_obj_x = 160;

void loop()
{
    const uint32_t start_frame_ms = millis(); 
    GO.JOY_X.readAxis();
    GO.JOY_Y.readAxis();
    GO.BtnB.read();

    Direction direction = Direction::NONE;
    if (GO.JOY_X.isAxisPressed() == 2)
    {
        player.set_animation_type(AnimationType::WALK);
        direction = Direction::LEFT;
    }

    if (GO.JOY_X.isAxisPressed() == 1)
    {
        player.set_animation_type(AnimationType::WALK);
        direction = Direction::RIGHT;
    }

    if (GO.JOY_Y.isAxisPressed() == 2)
    {
        player.set_animation_type(AnimationType::CLIMB);
        direction = Direction::UP;
    }

    if (GO.JOY_Y.isAxisPressed() == 1)
    {
        player.set_animation_type(AnimationType::CLIMB);
        direction = Direction::DOWN;
    }

    if (GO.BtnB.isPressed() == 1)
    {
        attack_frame_count = 6;
    }

    if (attack_frame_count > 0)
    {
        player.set_animation_type(AnimationType::ATTACK);
        --attack_frame_count;
    }

    if (direction != Direction::NONE)
    {
      player.set_direction(direction);
    }
    else if (player.type == AnimationType::WALK)
    {
        player.set_animation_type(AnimationType::IDLE);
    }

    Level::SurfacePoint next_point = level.get_next_character_point(pos_obj_x, pos_obj_y, direction, step);

    if (player.type == AnimationType::CLIMB && next_point.type != Level::STAIR)
    {
        player.set_animation_type(AnimationType::IDLE);
    }

    pos_obj_x = next_point.x;
    pos_obj_y = next_point.y;
    player.draw(pos_obj_x, next_point.y);

 
    screen.render();
    const uint32_t end_frame_ms = millis();
    delay(frame_time_ms - (end_frame_ms - start_frame_ms));
}
