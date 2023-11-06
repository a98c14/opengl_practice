#pragma once

#include <core/defines.h>
#include <core/math.h>
#include <gfx/base.h>
#include <engine/window.h>

typedef struct
{
    Vec2 world;
    Vec2 screen;
    float64 raw_x;
    float64 raw_y;
} InputMouse;

internal Vec2
mouse_world_position(Vec2 raw_mouse_pos, Camera camera);

internal InputMouse
input_mouse_get(Window* window, Camera camera);