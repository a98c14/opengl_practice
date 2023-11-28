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
    bool32 is_lock_to_parent = false;

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

    Joint joints[3] = {0};
    joints[0] = root;
    for(int i = 1; i < joint_count; i++)
    {
        joints[i].start = joints[i-1].end;
        joints[i].rotation = 90;
        joints[i].length = arm_length;
        joints[i].end = calculate_joint_end(joints[i]);
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
            Vec2 current_target = target;
            // reach forward
            for(int i = joint_count-1; i >= 0; i--)
            {
               joints[i].end = current_target;
               joints[i].start = add_vec2(current_target, scaled_heading_to_vec2(current_target, joints[i].start, joints[i].length));
               joints[i].rotation = angle_vec2(sub_vec2(joints[i].end, joints[i].start));
               current_target = joints[i].start;
            }

            current_target = root.start;
            // reach backward
            for(int i = 0; i < joint_count; i++)
            {
                joints[i].start = current_target;
                joints[i].end = add_vec2(current_target, scaled_heading_to_vec2(current_target, joints[i].end, joints[i].length));
                joints[i].rotation = angle_vec2(sub_vec2(joints[i].end, joints[i].start));
                current_target = joints[i].end;
            }
        }

        // draw
        for(int32 i = 0; i < joint_count; i++)
        {
            Vec2 position = joints[i].start;
            float32 angle = joints[i].rotation;
            draw_line(dc, joints[i].start, joints[i].end, ColorWhite, 2);
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
            ui_label(e->ctx, layout_grid_cell(layout, 0, 3), string("Lock Parent:"), t->label_default);
            ui_toggle(e->ctx, layout_grid_multicell(layout, 1, 3, 2, 1), uuid_new(5, 0), &is_lock_to_parent, t->toggle_default);
        }
        

        /** Control Panel */

        engine_render(e);
        editor_draw_stats(e);
    }

    window_destroy(e->window);
    logger_flush();
    return 0;
}