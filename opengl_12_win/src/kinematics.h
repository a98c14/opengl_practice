#pragma once
#include "app.h"
typedef struct
{
    Vec2 position;
    float32 default_rotation;
    float32 base_rotation;
    float32 local_rotation;
    float32 length;
    float32 rotation_constraint;
} Joint;

internal Joint 
joint(Vec2 pos, float32 rotation, float32 default_rotation, float32 length, float32 rotation_constraint);

internal Vec2
joint_end(Joint j);

internal Joint
joint_link_to(Joint child, Joint parent);

internal float32
joint_rotation(Joint j);

internal Joint
joint_rotate(Joint j, float32 angle);

internal void
draw_joint(Engine* e, Joint j);

internal void
draw_ghost_joint(Engine* e, Joint j);

internal void 
draw_arm(DrawContext* dc, Vec2 position, float32 rotation,float32 length);

internal void
fabrik_reach_forward(Vec2 target, Joint* joints, int32 joint_count, bool32 rotation_constraint);

internal void
fabrik_reach_backwards(Vec2 position, Joint* joints, int32 joint_count, bool32 rotation_constraint);

internal void
fix_base_rotation(Joint* child, Joint parent);