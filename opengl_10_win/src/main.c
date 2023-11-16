#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib,"user32.lib")

#include <opus.h>
#include "editor.h"
#include "app.h"

#include <opus.c>
#include "editor.c"
#include "app.c"

internal void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

#define WINDOW_WIDTH 2560
#define WINDOW_HEIGHT 1440
#define WORLD_WIDTH 640.0f
#define WORLD_HEIGHT 480.0f
#define PPU (1.0f / (WINDOW_WIDTH / WORLD_WITH)) // pixel per unit

int main(void)
{
    /* initialization */
    logger_init();

    Arena* persistent_arena = make_arena_reserve(mb(512));
    Arena* frame_arena = make_arena_reserve(mb(256));
    Window* window = window_create(persistent_arena, WINDOW_WIDTH, WINDOW_HEIGHT, "Simple Example", key_callback);
    RendererConfiguration renderer_configuration = {
        .window_width = WINDOW_WIDTH,
        .window_height = WINDOW_HEIGHT,
        .world_width = 0,
        .world_height = WORLD_HEIGHT,
        .clear_color = ColorSlate900
    };

    Renderer* renderer = renderer_new(persistent_arena, &renderer_configuration);
    DrawContext* dc = draw_context_new(persistent_arena, renderer);
    EngineTime time = engine_time_new();
    Theme* default_theme = theme_init_default(persistent_arena, renderer);
    Rect screen = rect_from_wh(renderer->world_width, renderer->world_height);
    UIContext* ctx = ui_context_new(persistent_arena, frame_arena, dc, default_theme);

    /* application state */
    // ShowCursor(0);

    Vec2 window_pos = vec2_zero();
    bool32 window_is_enabled = 1;
    float32 slider_value = 4;
    InputMouse mouse = {0};
    
    Profiler* main_frame = profiler_new(persistent_arena, string("Total"));
    Profiler* update = profiler_new(persistent_arena, string("Update"));
    Profiler* render = profiler_new(persistent_arena, string("Render"));
    const float32 cache_rate = 0.2;
    float32 cache_clock = 0;

    /* main loop */
    while (!glfwWindowShouldClose(window->glfw_window))
    {
        profiler_end(main_frame);
        profiler_begin(main_frame);
        /* frame: init */
        time = engine_get_time(time);
        mouse = input_mouse_get(window, renderer->camera, mouse);
        ctx->mouse = mouse;
        ctx->time = time;

        /* frame: update */
        profiler_begin(update);
        StyleRect test_style = default_theme->rect_default;
        test_style.color = color_to_vec4(ColorRed600);
        test_style.border_radius = default_theme->rounded_none;
        test_style.softness = 1;

        StyleRect header_style = default_theme->rect_default;
        header_style.color = color_to_vec4(ColorRed900);
        header_style.border_radius = (BorderRadius){ .bl = 0, .br = 0, .tr = 2, .tl = 2 };
        draw_line(dc, vec2(-50, 0), vec2(50, 0), ColorRed900, 1.6);
        draw_line(dc, vec2(0, -50), vec2(0, 50), ColorRed900, 1.6);
        if(ui_window_begin(ctx, string("Test"), &window_pos, vec2(100, 0), &window_is_enabled))
        {
            ui_text(ctx, string("First Line"));
            ui_text(ctx, string("Second Line"));
            ui_text(ctx, string("Third Line"));
            if(ui_button(ctx, string("Button")))
            {

            }
            if(ui_slider(ctx, string("Coords"), range(0, 10), &slider_value))
            {
                
            }
        }
        ui_window_end(ctx);

        profiler_end(update);
        /* frame: render */
        profiler_begin(render);
        renderer_render(renderer, time.dt);
        window_update(window);
        profiler_end(render);

        LayoutStack layout = layout_stack(rect(-200, -100, 100, 0), 10, vec2(4, 4), 4);
        draw_rect(dc, layout_stack_push(&layout), 0, 10, default_theme->rect_debug);
        draw_rect(dc, layout_stack_push(&layout), 0, 10, default_theme->rect_debug);
        draw_rect(dc, layout_stack_push(&layout), 0, 10, default_theme->rect_debug);
        ui_container(ctx, layout_stack_container(&layout), default_theme->container_light);
        
        bool32 is_expanded = false;
        ui_window(ctx, rect(200, 100, 100, 100), string("new window"), &is_expanded, default_theme->window_default);
        
        if(cache_clock > cache_rate)
        {
            profiler_refresh_cache(main_frame);
            profiler_refresh_cache(update);
            profiler_refresh_cache(render);
            cache_clock = 0;
        }
        
        editor_draw_stats(persistent_arena, frame_arena, renderer, ctx, default_theme, time, main_frame, update, render, screen, mouse);
        cache_clock += time.dt;
        arena_reset(frame_arena);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}