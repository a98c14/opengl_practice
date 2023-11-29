#include <corecrt_math.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib,"user32.lib")

#include <opus.h>
#include <opus.c>

#include "editor.h"
#include "app.h"
#include "kinematics.h"

#include "editor.c"
#include "app.c"
#include "kinematics.c"

int main(void)
{
    Engine* e = engine_init();
    DrawContext* dc = e->draw_context;
    Theme* t = e->theme; 

    /* app initialization */
    float32 dt_scale = 1;

    float32 world_width = e->renderer->world_width;
    float32 world_height = e->renderer->world_height;
    float32 world_half_width = world_width / 2.0f;
    float32 world_half_height = world_height / 2.0f;

    /* ui stuff */
    Rect window_rect = rect(200, 100, 180, 100);
    bool32 is_expanded = false;
    bool32 smooth_animations = false;
    bool32 rotation_animations = false;
    bool32 angle_constraints = true;

    const int32 joint_count = 3;
    const float32 arm_length = 100;
    const float32 thickness = 3;
    const Vec4 color = color_to_vec4(ColorWhite);
    float32 angles[3] = { 30, 60, 90};

    Joint root;
    root.start = vec2_zero();
    root.rotation = 0;
    root.length = arm_length;
    root.end = calculate_joint_end(root);

    Joint current_joints[3] = {0};
    Joint target_joints[3] = {0};
    current_joints[0] = root;
    target_joints[0] = root;
    for(int i = 1; i < joint_count; i++)
    {
        current_joints[i].start = current_joints[i-1].end;
        current_joints[i].rotation = 90;
        current_joints[i].length = arm_length;
        current_joints[i].end = calculate_joint_end(current_joints[i]);
        target_joints[i] = current_joints[i];
    }
    target_joints[0].min_angle = 0;
    target_joints[0].max_angle = 90;
    target_joints[1].min_angle = -120;
    target_joints[1].max_angle =  120;
    target_joints[2].min_angle = -180;
    target_joints[2].max_angle =  180;


    /* main loop */
    while (!glfwWindowShouldClose(e->window->glfw_window))
    {
        engine_frame_start(e);
        float32 dt = e->time.dt * dt_scale;

        Vec2 target = e->mouse.world;
        float32 max_reach_length = joint_count * arm_length;
        float32 distance_to_target = dist_vec2(root.start, target);
        target = scaled_heading_to_vec2(root.start, target, min(distance_to_target, max_reach_length));

        if(distance_to_target > 1)
        {
            if(angle_constraints)
            {
                // reach forward
                Vec2 current_target = target;
                for(int i = joint_count-1; i >= 0; i--)
                {
                    Joint* j = &target_joints[i];
                    j->end = current_target;
                    Vec2 current_arm = scaled_heading_to_vec2(current_target, j->start, j->length);
                    Vec2 next_arm = current_arm;
                    if(i < joint_count - 1) next_arm = sub_vec2(target_joints[i+1].end, target_joints[i+1].start);

                    float32 angle = angle_between_vec2(next_arm, current_arm);
                    if(angle > j->max_angle || angle < j->min_angle)
                    {
                        float32 new_angle = clamp(j->min_angle, angle, j->max_angle);
                        float32 diff = angle - new_angle; 
                        angle = new_angle;
                        current_arm = rotate_vec2(current_arm, diff);
                    }
                    j->start = add_vec2(current_target, current_arm);
                    j->local_rotation = angle;
                    current_target = j->start;
                }

                // reach backward
                current_target = root.start;
                for(int i = 0; i < joint_count; i++)
                {
                    Joint* j = &target_joints[i];
                    j->start = current_target;
                    Vec2 current_arm = scaled_heading_to_vec2(current_target, j->end, j->length);
                    Vec2 previous_arm =  vec2_right();
                    if(i > 0) previous_arm = sub_vec2(target_joints[i-1].end, target_joints[i-1].start);
                    float32 base_rotation = 0;
                    if(i > 0) base_rotation = target_joints[i-1].rotation;

                    float32 angle = angle_between_vec2(current_arm, previous_arm);
                    if(angle > j->max_angle || angle < j->min_angle)
                    {
                        float32 new_angle = clamp(j->min_angle, angle, j->max_angle);
                        float32 diff = new_angle - angle; 
                        angle = new_angle;
                        current_arm = rotate_vec2(current_arm, diff);
                    }
                    j->end = add_vec2(current_target, current_arm);
                    j->rotation = base_rotation + angle;
                    j->local_rotation = angle;
                    current_target = j->end;
                }
            }
            else
            {
                Vec2 current_target = target;
                
                // reach forward
                for(int i = joint_count-1; i >= 0; i--)
                {
                target_joints[i].end = current_target;
                target_joints[i].start = add_vec2(current_target, scaled_heading_to_vec2(current_target, target_joints[i].start, target_joints[i].length));
                target_joints[i].rotation = angle_vec2(sub_vec2(target_joints[i].end, target_joints[i].start));
                current_target = target_joints[i].start;
                }

                current_target = root.start;
                // reach backward
                for(int i = 0; i < joint_count; i++)
                {
                    target_joints[i].start = current_target;
                    target_joints[i].end = add_vec2(current_target, scaled_heading_to_vec2(current_target, target_joints[i].end, target_joints[i].length));
                    target_joints[i].rotation = angle_vec2(sub_vec2(target_joints[i].end, target_joints[i].start));
                    current_target = target_joints[i].end;
                }
            }
        }

        // interpolate for smoother animations
        if(smooth_animations)
        {
            for(int i = 0; i < joint_count; i++)
            {
                current_joints[i].start = lerp_vec2(current_joints[i].start, target_joints[i].start, dt * 8);
                current_joints[i].end = lerp_vec2(current_joints[i].end, target_joints[i].end, dt * 8);
                current_joints[i].rotation = lerp_f32(current_joints[i].rotation, target_joints[i].rotation, dt * 8);
                current_joints[i].local_rotation = lerp_f32(current_joints[i].local_rotation, target_joints[i].local_rotation, dt * 8);
                current_joints[i].min_angle = lerp_f32(current_joints[i].min_angle, target_joints[i].min_angle, dt * 8);
                current_joints[i].max_angle = lerp_f32(current_joints[i].max_angle, target_joints[i].max_angle, dt * 8);
            }
        }
        // interpolate the rotations only, then calculate the positions from the rotations, prevents arm stretching
        else if(rotation_animations)
        {
            Vec2 current_position = root.start;
            for(int i = 0; i < joint_count; i++)
            {
                current_joints[i].rotation = lerp_f32(current_joints[i].rotation, target_joints[i].rotation, dt * 8);
                current_joints[i].local_rotation = lerp_f32(current_joints[i].local_rotation, target_joints[i].local_rotation, dt * 8);
                current_joints[i].start = current_position;
                current_joints[i].end = add_vec2(current_position, vec2_heading_scaled(current_joints[i].rotation, current_joints[i].length));
                current_position = current_joints[i].end;
            }
        }
        // no smoothing
        else
        {
            for(int i = 0; i < joint_count; i++)
            {
                current_joints[i].start = target_joints[i].start;
                current_joints[i].end = target_joints[i].end;
                current_joints[i].rotation = target_joints[i].rotation;
                current_joints[i].local_rotation = target_joints[i].local_rotation;
                current_joints[i].min_angle = target_joints[i].min_angle;
                current_joints[i].max_angle = target_joints[i].max_angle;
            }
        }

        // draw
        for(int32 i = 0; i < joint_count; i++)
        {
            draw_joint(e, current_joints[i]);
        }

        UIWindow window = ui_window(e->ctx, &window_rect, uuid_new(1, 0), string("simulation"), &is_expanded, t->window_default);
        if(window.is_expanded)
        {
            uint32 row_count = 6;
            LayoutGrid layout = layout_grid(rect_anchor(rect_from_wh(window.header.w, em(2) * row_count), window.header, ANCHOR_TL_TL), 4, row_count, e->theme->p2);
            ui_container(e->ctx, layout_grid_container(layout), t->container_light);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 0), string_pushf(e->frame_arena, "Angle [0]: %0.2f", angles[0]), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 2, 0, 2, 1), uuid_new(2, 0), range(-360, 360), &angles[0], t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 1), string_pushf(e->frame_arena, "Angle [1]: %0.2f", angles[1]), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 2, 1, 2, 1), uuid_new(3, 0), range(-360, 360), &angles[1], t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 2), string_pushf(e->frame_arena, "Angle [2] %0.2f", angles[2]), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 2, 2, 2, 1), uuid_new(4, 0), range(-360, 360), &angles[2], t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 3), string("Smooth animations:"), t->label_default);
            ui_toggle(e->ctx, layout_grid_multicell(layout, 2, 3, 1, 1), uuid_new(5, 0), &smooth_animations, t->toggle_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 4), string("Rotation Constraints:"), t->label_default);
            ui_toggle(e->ctx, layout_grid_multicell(layout, 2, 4, 1, 1), uuid_new(6, 0), &angle_constraints, t->toggle_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 5), string("Rotation Animations:"), t->label_default);
            ui_toggle(e->ctx, layout_grid_multicell(layout, 2, 5, 1, 1), uuid_new(7, 0), &rotation_animations, t->toggle_default);
        }
        

        /** Control Panel */

        engine_render(e);
        editor_draw_stats(e);
    }

    window_destroy(e->window);
    logger_flush();
    return 0;
}