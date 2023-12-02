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
    bool32 distance_check = true;


    float32 angles[3] = { 30, 60, 90};

    // Joint_Deprecated root;
    // root.start = vec2(100, 0);
    // root.rotation = 0;
    // root.length = arm_length;
    // root.end = calculate_joint_end(root);

    // Joint_Deprecated current_joints[3] = {0};
    // Joint_Deprecated target_joints[3] = {0};
    // Joint_Deprecated temp_joints[3] = {0};
    // current_joints[0] = root;
    // target_joints[0] = root;
    // for(int i = 1; i < joint_count; i++)
    // {
    //     current_joints[i].start = current_joints[i-1].end;
    //     current_joints[i].rotation = 90;
    //     current_joints[i].length = arm_length;
    //     current_joints[i].end = calculate_joint_end(current_joints[i]);
    //     target_joints[i] = current_joints[i];
    // }
    // target_joints[0].min_angle = -180;
    // target_joints[0].max_angle = 180;
    // target_joints[1].min_angle = -120;
    // target_joints[1].max_angle =  120;
    // target_joints[2].min_angle = -90;
    // target_joints[2].max_angle =  90;
    // memcpy(temp_joints, target_joints, sizeof(target_joints));

    const float32 arm_length = 100;
    Joint joints[3] = {0};
    int32 joint_count = array_count(joints);
    float32 reach_threshold = 1;

    joints[0] = joint(vec2(-200, 0), 90, 0, arm_length);
    joints[1] = joint(vec2_zero(), 90, 0, arm_length);
    joints[2] = joint(vec2_zero(), 90, 0, arm_length);
    Joint root = joints[0];

    /* main loop */
    while (!glfwWindowShouldClose(e->window->glfw_window))
    {
        engine_frame_start(e);
        float32 dt = e->time.dt * dt_scale;

        Vec2 target = e->mouse.world;
        float32 max_reach_length = joint_count * arm_length;
        float32 distance_to_target = dist_vec2(root.position, target);
        if(distance_to_target > max_reach_length)
            target = move_towards_vec2(root.position, target, max_reach_length);
        draw_circle_filled(dc, circle(target, 5), ColorRed300);
        draw_line(dc, root.position, e->mouse.world, ColorSlate500, 2);

        if(distance_to_target > reach_threshold)
        {
            fabrik_reach_forward(target, joints, joint_count);
        }

        // draw
        for(int32 i = 0; i < joint_count; i++)
        {
            draw_joint(e, joints[i]);
        }

        UIWindow window = ui_window(e->ctx, &window_rect, uuid_new(1, 0), string("simulation"), &is_expanded, t->window_default);
        if(window.is_expanded)
        {
            uint32 row_count = 7;
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
            ui_label(e->ctx, layout_grid_cell(layout, 0, 6), string("Distance Check:"), t->label_default);
            ui_toggle(e->ctx, layout_grid_multicell(layout, 2, 6, 1, 1), uuid_new(8, 0), &distance_check, t->toggle_default);
        }
        

        /** Control Panel */

        engine_render(e);
        editor_draw_stats(e);
    }

    window_destroy(e->window);
    logger_flush();
    return 0;
}