#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib,"user32.lib")

#include <opus.h>
#include "editor.h"
#include "app.h"

#include <opus.c>
#include "editor.c"
#include "app.c"

int main(void)
{
    Engine* e = engine_init();
    DrawContext* dc = e->draw_context;
    Theme* t = e->theme;

    /* app initialization */
    Rect window_rect = rect(200, 100, 70, 100);
    bool32 is_expanded = false;
    float32 slider_value = 4;

    int32 boid_count = 1;
    Vec2* positions = arena_push_array_zero(e->persistent_arena, Vec2, boid_count);
    Vec2* directions = arena_push_array_zero(e->persistent_arena, Vec2, boid_count);
    float32* rotations = arena_push_array_zero(e->persistent_arena, float32, boid_count);

    for(int32 i = 0; i < boid_count; i++)
    {
        directions[i] = mul_vec2_f32(vec2_right(), 10);
    }

    /* main loop */
    while (!glfwWindowShouldClose(e->window->glfw_window))
    {
        engine_frame_start(e);

        /* move */
        for(int32 i = 0; i < boid_count; i++ )
            positions[i] = add_vec2(positions[i], mul_vec2_f32(directions[i], e->time.dt));

        /* calculate rotations */
        for(int32 i = 0; i < boid_count; i++ )
            rotations[i] = angle_vec2(directions[i]);

        /* draw */
        for(int32 i = 0; i < boid_count; i++ )
            draw_boid(dc, positions[i], rotations[i], 5, ColorWhite);

        /* editor */
        UIWindow window = ui_window(e->ctx, &window_rect, uuid_new(1, 0), string("new window"), &is_expanded, t->window_default);
        if(window.is_expanded)
        {
            LayoutStack layout = layout_stack(window.header, em(1), vec2(4, 4), 2);
            ui_label(e->ctx, layout_stack_push(&layout), string("First line"), t->label_default);
            ui_label(e->ctx, layout_stack_push(&layout), string("Second line"), t->label_default);
            ui_label(e->ctx, layout_stack_push(&layout), string_pushf(e->frame_arena, "Slider: %0.2f", slider_value), t->label_default);
            ui_slider(e->ctx, layout_stack_push(&layout), uuid_new(2, 0), string("Coords"), range(0, 10), &slider_value, t->slider_default);
            if(ui_button(e->ctx, layout_stack_push_scaled(&layout, 1.5), uuid_new(3, 0), string("Button"), t->button_default))
            {
                log_info("Clicked");
            }
            ui_label(e->ctx, layout_stack_push(&layout), string("Padtest"), t->label_default);
            ui_container(e->ctx, layout_stack_container(&layout), t->container_light);
        }

        engine_render(e);
        editor_draw_stats(e);
    }

    window_destroy(e->window);
    logger_flush();
    return 0;
}