#include "kinematics.h"

internal Joint 
joint(Vec2 pos, float32 rotation, float32 default_rotation, float32 length, float32 rotation_constraint)
{
    Joint result;
    result.position = pos;
    result.base_rotation = 0;
    result.local_rotation = rotation;
    result.default_rotation = default_rotation;
    result.length = length;
    result.rotation_constraint = rotation_constraint;
    return result;
}

internal Vec2
joint_end(Joint j)
{
    Vec2 result = add_vec2(j.position, heading_vec2_scaled(joint_rotation(j), j.length));
    return result;
}

internal Joint
joint_link_to(Joint child, Joint parent)
{
    Joint result;
    result.position = add_vec2(joint_end(parent), child.position);
    result.base_rotation = joint_rotation(parent) - 180;
    result.local_rotation = joint_rotation(child);
    result.default_rotation = child.default_rotation;
    result.length = child.length;
    return result;
}

internal float32
joint_rotation(Joint j)
{
    return j.base_rotation + j.default_rotation + j.local_rotation;
}

internal Joint
joint_rotate(Joint j, float32 angle)
{
    j.local_rotation = angle - j.base_rotation - j.default_rotation;
    if(j.local_rotation < 0) j.local_rotation = 360 + j.local_rotation;
    return j;
}

internal void
draw_joint(Engine* e, Joint j)
{
    draw_line(e->draw_context, j.position, joint_end(j), ColorWhite, 2);
    draw_circle_partially_filled(e->draw_context, j.position, j.base_rotation, 20, ColorAmber700A, 0,  joint_rotation(j) - j.base_rotation);
    draw_text(e->draw_context, vec2(j.position.x + 5, j.position.y + 10), string_pushf(e->frame_arena, "Local: %0.2f", j.local_rotation), AlignmentBottomLeft, e->theme->font_default_light);
    draw_text(e->draw_context, vec2(j.position.x + 5, j.position.y + 15), string_pushf(e->frame_arena, "Base: %0.2f", j.base_rotation), AlignmentBottomLeft, e->theme->font_default_light);
    draw_text(e->draw_context, vec2(j.position.x + 5, j.position.y + 20), string_pushf(e->frame_arena, "Default: %0.2f", j.default_rotation), AlignmentBottomLeft, e->theme->font_default_light);
}

internal void 
draw_joint_deprecated(Engine* e, Joint_Deprecated j)
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

internal void
fabrik_reach_forward(Vec2 target, Joint* joints, int32 joint_count)
{
    Vec2 current_target = target;
    for(int i = joint_count-1; i >= 0; i--)
    {
        Joint j = joints[i];
        j.position = move_towards_vec2(current_target, j.position, j.length);
        j = joint_rotate(j, angle_vec2(sub_vec2(current_target, j.position)));
        if(i < joint_count - 1) 
        {
            fix_base_rotation(&joints[i+1], j);
            // rotation constraint
            float32 diff = joints[i+1].local_rotation - min(joints[i+1].local_rotation, joints[i+1].rotation_constraint);
            if(diff > 0)
            {
                j.position = add_vec2(current_target, rotate_vec2(sub_vec2(j.position, current_target), diff));
                j = joint_rotate(j, angle_vec2(sub_vec2(current_target, j.position)));
                fix_base_rotation(&joints[i+1], j);
            }
        }
        joints[i] = j;
        current_target = j.position;
    }
}

internal void
fabrik_reach_backwards(Vec2 position, Joint* joints, int32 joint_count)
{
    Vec2 new_start_position = position;
    for(int i = 0; i < joint_count; i++)
    {
        Joint j = joints[i];
        Vec2 old_end_position = joint_end(j);
        float32 new_rotation = angle_vec2(sub_vec2(old_end_position, new_start_position));
        j.position = new_start_position;
        j = joint_rotate(j, new_rotation);
        new_start_position = joint_end(j);

        // rotation constraint
        float32 diff = j.local_rotation - min(j.local_rotation, j.rotation_constraint);
        if(diff > 0)
        {
            new_start_position = add_vec2(j.position, rotate_vec2(sub_vec2(new_start_position, j.position), -diff));
            j = joint_rotate(j, angle_vec2(sub_vec2(new_start_position, j.position)));
        }

        if(i < joint_count - 1) 
        {
            fix_base_rotation(&joints[i+1], j);            
        }

        joints[i] = j;
    }
}

internal Joint
lerp_joint(Joint a, Joint b, float32 dt)
{
    Joint result;
    result.base_rotation = lerp_f32(a.base_rotation, b.base_rotation, dt);
    result.local_rotation = lerp_f32(a.local_rotation, b.local_rotation, dt);
    result.default_rotation = lerp_f32(a.default_rotation, b.default_rotation, dt);
    result.length = lerp_f32(a.length, b.length, dt);
    result.position = b.position;
    return result;
}

/** Fixes the base rotation for the child. Needs to be called
 * when parent rotation changes so it propagates to the child.
 */
internal void
fix_base_rotation(Joint* child, Joint parent)
{
    float32 new_base_rotation = joint_rotation(parent) - 180;
    child->local_rotation = child->local_rotation + child->base_rotation - new_base_rotation;
    if(child->local_rotation < 0) child->local_rotation = 360 + child->local_rotation;
    child->base_rotation = new_base_rotation;    
}


internal Vec2
calculate_joint_start(Joint_Deprecated j)
{
    Vec2 result = add_vec2(j.start, heading_inverse_vec2_scaled(j.rotation, j.length));
    return result;
}

internal Vec2
calculate_joint_end(Joint_Deprecated j)
{
    Vec2 result = add_vec2(j.start, heading_vec2_scaled(j.rotation, j.length));
    return result;
}