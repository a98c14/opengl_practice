#include "kinematics.h"

internal void 
draw_joint(Engine* e, Joint j)
{
    float32 angle = j.rotation;
    draw_circle_partially_filled(e->draw_context, j.start, j.rotation - j.local_rotation, 20, ColorAmber700A, j.min_angle, j.max_angle);
    draw_line(e->draw_context, j.start, j.end, ColorWhite, 2);
    draw_arrow(e->draw_context, j.start, 20, angle+90, ColorBlue500, 2);
    draw_arrow(e->draw_context, j.start, 20, angle, ColorRed500, 2);
    draw_circle(e->draw_context, j.start, 20, ColorWhite);
    draw_text(e->draw_context, vec2(j.start.x, j.start.y + 25), string_pushf(e->frame_arena, "Base: %0.2f", j.rotation), AlignmentBottom, e->theme->font_default_light);
    draw_text(e->draw_context, vec2(j.start.x, j.start.y + 20), string_pushf(e->frame_arena, "Local: %0.2f", j.local_rotation), AlignmentBottom, e->theme->font_default_light);
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