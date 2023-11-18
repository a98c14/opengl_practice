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
    Engine* engine = engine_init();
    DrawContext* dc = engine->draw_context;
    Theme* t = engine->theme;

    /* app initialization */
    Rect window_rect = rect(200, 100, 70, 100);
    bool32 is_expanded = false;
    float32 slider_value = 4;

    /* main loop */
    while (!glfwWindowShouldClose(engine->window->glfw_window))
    {
        engine_frame_start(engine);
        draw_boid(dc, vec2(10, 10), vec2_zero(), 40, ColorWhite);
        draw_boid(dc, vec2(30, 10), vec2_zero(), 40, ColorWhite);
        draw_boid(dc, vec2(10, 30), vec2_zero(), 40, ColorWhite);

        UIWindow window = ui_window(engine->ctx, &window_rect, uuid_new(1, 0), string("new window"), &is_expanded, t->window_default);
        if(window.is_expanded)
        {
            LayoutStack layout = layout_stack(window.header, em(1), vec2(4, 4), 2);
            ui_label(engine->ctx, layout_stack_push(&layout), string("First line"), t->label_default);
            ui_label(engine->ctx, layout_stack_push(&layout), string("Second line"), t->label_default);
            ui_label(engine->ctx, layout_stack_push(&layout), string_pushf(engine->frame_arena, "Slider: %0.2f", slider_value), t->label_default);
            ui_slider(engine->ctx, layout_stack_push(&layout), uuid_new(2, 0), string("Coords"), range(0, 10), &slider_value, t->slider_default);
            if(ui_button(engine->ctx, layout_stack_push_scaled(&layout, 1.5), uuid_new(3, 0), string("Button"), t->button_default))
            {
                log_info("Clicked");
            }
            ui_label(engine->ctx, layout_stack_push(&layout), string("Padtest"), t->label_default);
            ui_container(engine->ctx, layout_stack_container(&layout), t->container_light);
        }

        engine_render(engine);
        editor_draw_stats(engine);
    }

    window_destroy(engine->window);
    logger_flush();
    return 0;
}