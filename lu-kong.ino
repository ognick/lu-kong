#include <odroid_go.h>
#include "image.h"
#include "screen.h"
#include "character.h"



extern const Image player_walk_img;
extern const Image player_idle_img;
extern const Image player_climb_img;
extern const Image player_attack_img;
extern const Image bg_img;

Screen screen(bg_img);
Character player(screen);



void setup()
{
    GO.begin();
    screen.init();
    player.add_animation(AnimationType::IDLE, player_idle_img, 4);
    player.add_animation(AnimationType::WALK, player_walk_img, 6);
    player.add_animation(AnimationType::CLIMB, player_climb_img, 6);
    player.add_animation(AnimationType::ATTACK, player_attack_img, 6);
}

const uint8_t fps = 15;
const uint32_t frame_time_ms = static_cast<uint32_t>(1 / static_cast<float>(fps) * 1000);
const uint16_t step = 6;
size_t position = 1;
int32_t pos_obj_y = 160;
int8_t horizontal_direction = 0;
int8_t vertical_direction = 0;
uint8_t attack_frame_count = 0;
int32_t pos_obj_x = 160;

void loop()
{
    const uint32_t start_frame_ms = millis();
    GO.JOY_X.readAxis();
    GO.JOY_Y.readAxis();
    GO.BtnB.read();

    if (GO.JOY_X.isAxisPressed() == 2)
    {
        horizontal_direction = -1;
        player.set_animation_type(AnimationType::WALK);
        player.set_direction(Direction::LEFT);
    }

    if (GO.JOY_X.isAxisPressed() == 1)
    {
        horizontal_direction = 1;
        player.set_animation_type(AnimationType::WALK);
        player.set_direction(Direction::RIGHT);
    }

    if (GO.JOY_Y.isAxisPressed() == 2)
    {
        vertical_direction = -1;
        player.set_animation_type(AnimationType::CLIMB);
        player.set_direction(Direction::UP);
    }

    if (GO.JOY_Y.isAxisPressed() == 1)
    {
        vertical_direction = 1;
        player.set_animation_type(AnimationType::CLIMB);
        player.set_direction(Direction::DOWN);
    }

    if (GO.BtnB.isPressed() == 1)
    {
        attack_frame_count = 6;
    }

    pos_obj_x += horizontal_direction * step;
    pos_obj_y += vertical_direction * step;

    if (attack_frame_count > 0)
    {
        player.set_animation_type(AnimationType::ATTACK);
        --attack_frame_count;
    }

    if (horizontal_direction == 0 && vertical_direction == 0 && attack_frame_count == 0)
    {
        player.set_animation_type(AnimationType::IDLE);
    }

    player.draw(pos_obj_x, pos_obj_y);

    horizontal_direction = 0;
    vertical_direction = 0;
    screen.render();
    const uint32_t end_frame_ms = millis();
    delay(frame_time_ms - (end_frame_ms - start_frame_ms));
}
