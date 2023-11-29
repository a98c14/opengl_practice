#pragma once
#include "app.h"

typedef struct
{
    Vec2 start;
    Vec2 end;
    float32 rotation;
    float32 length; 

    float32 local_rotation; // TEMP:
} Joint;

internal void 
draw_joint(DrawContext* dc, Vec2 position, float32 rotation, float32 radius);


internal void 
draw_arm(DrawContext* dc, Vec2 position, float32 rotation,float32 length);

internal Vec2
calculate_joint_end(Joint j);