#pragma once
#include "app.h"

typedef struct
{
    Vec2 start;
    Vec2 end;
    float32 rotation;
    float32 length; 

    float32 local_rotation; // TEMP:

    // constraints
    float32 min_angle;
    float32 max_angle;
} Joint;

internal void 
draw_joint(Engine* e, Joint j);

internal void 
draw_arm(DrawContext* dc, Vec2 position, float32 rotation,float32 length);

internal Vec2
calculate_joint_end(Joint j);