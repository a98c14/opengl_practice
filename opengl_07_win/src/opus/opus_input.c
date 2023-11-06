#include "opus_input.h"

internal Vec2
mouse_world_position(Vec2 raw_mouse_pos, Camera camera)
{
    Vec2 mouse_world = vec2(-raw_mouse_pos.x / camera.window_width, raw_mouse_pos.y / camera.window_height);
    mouse_world = sub_vec2(vec2(0, 1), mouse_world);
    mouse_world = add_vec2_f32(mouse_world,-0.5);
    mouse_world.x *= camera.world_width;
    mouse_world.y *= camera.world_height;
    return mouse_world;
}