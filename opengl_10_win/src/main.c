#include <opus.h>
#include <opus.c>
#include "app.h"
#include "app.c"

internal void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

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
        .world_height = 480,
        .clear_color = ColorSlate900
    };

    Renderer* renderer = renderer_new(persistent_arena, &renderer_configuration);
    DrawContext* dc = draw_context_new(persistent_arena, renderer);
    EngineTime time = engine_time_new();
    Theme* default_theme = theme_init_default(persistent_arena, renderer);

    /* application state */

    /* main loop */
    while (!glfwWindowShouldClose(window->glfw_window))
    {
        Profiler main_frame = profiler_begin(string("MainFrame"));
        /* frame: init */
        time = engine_get_time(time);
        InputMouse mouse = input_mouse_get(window, renderer->camera);

        /* frame: update */
        Profiler update = profiler_begin(string("Update"));
        draw_debug_line(dc, vec2(0, 0), vec2(0, 100 * renderer->pixel_per_unit), ColorBlack);
        draw_debug_line(dc, vec2(0, 0), vec2(0, 200 * renderer->pixel_per_unit), ColorBlack);

        draw_rect(dc, rect(0, 0, 200, 100), 0, 0, default_theme->rect_default);
        draw_text(dc, vec2(0, 0), string("Sample Text"), default_theme->font_default);

        // UIContext* ctx;
        // ui_view_begin(ctx);
        //     ui_button(ctx);
        // ui_view_end();

        profiler_end(&update);
        /* frame: render */
        Profiler render = profiler_begin(string("Render"));
        renderer_render(renderer, time.dt);
        window_update(window);
        profiler_end(&render);
        profiler_end(&main_frame);
        arena_reset(frame_arena);

        float32 screen_left = -renderer->camera.world_width / 2;
        float32 screen_top = renderer->camera.world_height / 2;
        draw_text(dc, vec2(screen_left, screen_top-default_theme->font_debug.font_size*1), string_pushf(frame_arena, "%s: %0.02fms", main_frame.name.value, 1000*(main_frame.end - main_frame.start)), default_theme->font_debug);
        draw_text(dc, vec2(screen_left, screen_top-default_theme->font_debug.font_size*2), string_pushf(frame_arena, "%s: %0.02fms", update.name.value, 1000*(update.end - update.start)), default_theme->font_debug);
        draw_text(dc, vec2(screen_left, screen_top-default_theme->font_debug.font_size*3), string_pushf(frame_arena, "%s: %0.02fms", render.name.value, 1000*(render.end - render.start)), default_theme->font_debug);
        draw_text(dc, vec2(screen_left, screen_top-default_theme->font_debug.font_size*4), string_pushf(frame_arena, "Mouse: [%0.02f, %0.02f]", mouse.world.x, mouse.world.y), default_theme->font_debug);
        draw_text(dc, vec2(screen_left, screen_top-default_theme->font_debug.font_size*5), string_pushf(frame_arena, "MouseButtonState: 0x%x", mouse.button_state), default_theme->font_debug);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}