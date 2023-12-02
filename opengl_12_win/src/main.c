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
    bool32 smooth_animations = true;
    bool32 angle_constraints = true;
    bool32 distance_check = true;

    const float32 arm_length = 100;
    int32 joint_count = 2;
    Joint* joints = arena_push_array_zero(e->persistent_arena, Joint, joint_count);
    Joint* temp_joints = arena_push_array_zero(e->persistent_arena, Joint, joint_count);
    Joint* visual_joints = arena_push_array_zero(e->persistent_arena, Joint, joint_count);
    float32 reach_threshold = 1;

    joints[0] = joint(vec2(-200, 0), 90, 0, arm_length, 180);
    joints[1] = joint(vec2_zero(), 90, -90, arm_length, 180);
    // joints[2] = joint(vec2_zero(), 90, 0, arm_length, 360);
    Joint root = joints[0];
    memcpy(temp_joints, joints, joint_count * sizeof(Joint));
    memcpy(visual_joints, joints, joint_count * sizeof(Joint));

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
            if(!distance_check)
            {
                fabrik_reach_forward(target, joints, joint_count);
                fabrik_reach_backwards(root.position, joints, joint_count);
            }
            else
            {
                float32 end_distance_to_target = distsqr_vec2(joint_end(joints[joint_count-1]), target);
                fabrik_reach_forward(target, temp_joints, joint_count);
                fabrik_reach_backwards(root.position, temp_joints, joint_count);
                float32 new_end_distance_to_target = distsqr_vec2(joint_end(temp_joints[joint_count-1]), target);
                if(new_end_distance_to_target < end_distance_to_target)
                {
                    memcpy(joints, temp_joints, joint_count * sizeof(Joint));
                }
            }
        }

        if(smooth_animations)
        {
            for(int32 i = 0; i < joint_count; i++)
            {
                visual_joints[i] = lerp_joint(visual_joints[i], joints[i], dt*8);
                if(i > 0) visual_joints[i].position = joint_end(visual_joints[i-1]);
            }
        }
        else
        {
            memcpy(visual_joints, joints, joint_count * sizeof(Joint));
        }

        // draw
        for(int32 i = 0; i < joint_count; i++)
        {
            draw_joint(e, visual_joints[i]);
        }

        UIWindow window = ui_window(e->ctx, &window_rect, uuid_new(1, 0), string("simulation"), &is_expanded, t->window_default);
        if(window.is_expanded)
        {
            uint32 row_count = 7;
            LayoutGrid layout = layout_grid(rect_anchor(rect_from_wh(window.header.w, em(2) * row_count), window.header, ANCHOR_TL_TL), 4, row_count, e->theme->p2);
            ui_container(e->ctx, layout_grid_container(layout), t->container_light);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 3), string("Smooth animations:"), t->label_default);
            ui_toggle(e->ctx, layout_grid_multicell(layout, 2, 3, 1, 1), uuid_new(5, 0), &smooth_animations, t->toggle_default);
            ui_label(e->ctx, layout_grid_cell(layout, 0, 4), string("Rotation Constraints:"), t->label_default);
            ui_toggle(e->ctx, layout_grid_multicell(layout, 2, 4, 1, 1), uuid_new(6, 0), &angle_constraints, t->toggle_default);
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