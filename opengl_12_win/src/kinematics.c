#include "kinematics.h"

internal void 
draw_joint(DrawContext* dc, Vec2 position, float32 rotation, float32 radius)
{
    draw_circle(dc, position, radius, ColorWhite);
}

internal void 
draw_arm(DrawContext* dc, Vec2 position, float32 rotation,float32 length)
{

}

internal Vec2
calculate_joint_start(Joint j)
{
    Vec2 result = add_vec2(j.start, vec2_inverse_heading_scaled(j.rotation, j.length));
    return result;
}

internal Vec2
calculate_joint_end(Joint j)
{
    Vec2 result = add_vec2(j.start, vec2_heading_scaled(j.rotation, j.length));
    return result;
}