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
                Vec2 current_target = target;
                
                // reach forward
                for(int i = joint_count-1; i >= 0; i--)
                {
                    target_joints[i].end = current_target;
                    target_joints[i].start = add_vec2(current_target, scaled_heading_to_vec2(current_target, target_joints[i].start, target_joints[i].length));
                    float32 new_angle = angle_vec2(sub_vec2(target_joints[i].end, target_joints[i].start));

                    float32 base_rotation = 0;
                    if(i > 0) base_rotation = target_joints[i-1].rotation;

                    if(new_angle - base_rotation > 180 || new_angle - base_rotation < 0)
                    {
                        new_angle = base_rotation + clamp(0, new_angle - base_rotation, 180);
                        target_joints[i].start = add_vec2(current_target, vec2_inverse_heading_scaled(new_angle, target_joints[i].length));
                    }

                    target_joints[i].rotation = new_angle;
                    current_target = target_joints[i].start;
                }

                current_target = root.start;

                // reach backward
                for(int i = 0; i < joint_count; i++)
                {
                    target_joints[i].start = current_target;
                    target_joints[i].end = add_vec2(current_target, scaled_heading_to_vec2(current_target, target_joints[i].end, target_joints[i].length));
                    float32 new_angle = angle_vec2(sub_vec2(target_joints[i].end, target_joints[i].start));

                    float32 base_rotation = 0;
                    if(i > 0) base_rotation = target_joints[i-1].rotation;

                    if(new_angle - base_rotation > 180 || new_angle - base_rotation < 0)
                    {
                        new_angle = base_rotation + clamp(0, new_angle - base_rotation, 180);
                        target_joints[i].end = add_vec2(current_target, vec2_heading_scaled(new_angle, target_joints[i].length));
                    }
                    
                    target_joints[i].rotation = new_angle;
                    current_target = target_joints[i].end;
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
            }
        }
        else
        {
            for(int i = 0; i < joint_count; i++)
            {
                current_joints[i].start = target_joints[i].start;
                current_joints[i].end = target_joints[i].end;
                current_joints[i].rotation = target_joints[i].rotation;
            }
        }

        // draw
        for(int32 i = 0; i < joint_count; i++)
        {
            Vec2 position = current_joints[i].start;
            float32 angle = current_joints[i].rotation;
            draw_line(dc, current_joints[i].start, current_joints[i].end, ColorWhite, 2);
            draw_arrow(dc, position, 20, angle+90, ColorBlue500, 2);
            draw_arrow(dc, position, 20, angle, ColorRed500, 2);
            draw_circle(dc, position, 20, ColorWhite);
        }

        UIWindow window = ui_window(e->ctx, &window_rect, uuid_new(1, 0), string("simulation"), &is_expanded, t->window_default);
        if(window.is_expanded)
        {
            uint32 row_count = 4;
            LayoutGrid layout = layout_grid(rect_anchor(rect_from_wh(window.header.w, em(2) * row_count), window.header, ANCHOR_TL_TL), 3, row_count, e->theme->p2);
            ui_container(e->ctx, layout_grid_container(layout), t->container_light);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 0), string_pushf(e->frame_arena, "Angle [0]: %0.2f", angles[0]), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 1, 0, 2, 1), uuid_new(2, 0), range(-360, 360), &angles[0], t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 1), string_pushf(e->frame_arena, "Angle [1]: %0.2f", angles[1]), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 1, 1, 2, 1), uuid_new(3, 0), range(-360, 360), &angles[1], t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 2), string_pushf(e->frame_arena, "Angle [2] %0.2f", angles[2]), t->label_default);
            ui_slider(e->ctx, layout_grid_multicell(layout, 1, 2, 2, 1), uuid_new(4, 0), range(-360, 360), &angles[2], t->slider_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 3), string("Smooth animations:"), t->label_default);
            ui_toggle(e->ctx, layout_grid_multicell(layout, 1, 3, 2, 1), uuid_new(5, 0), &smooth_animations, t->toggle_default);
        }
        

        /** Control Panel */

        engine_render(e);
        editor_draw_stats(e);
    }

    window_destroy(e->window);
    logger_flush();
    return 0;
}