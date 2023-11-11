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

        draw_rect(dc, rect(0, -20, 20, 10), 0, ColorAmber300, 0);
        draw_rect(dc, rect(0, -60, 200, 100), 30, ColorAmber500, 0);

        

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
        const float32 debug_font_size = 20.0f * renderer->pixel_per_unit;
        Color debug_font_color = ColorWhite;
        draw_text(dc, vec2(screen_left, screen_top-debug_font_size*1), string_pushf(frame_arena, "%s: %0.02fms", main_frame.name.value, 1000*(main_frame.end - main_frame.start)), debug_font_color, debug_font_size);
        draw_text(dc, vec2(screen_left, screen_top-debug_font_size*2), string_pushf(frame_arena, "%s: %0.02fms", update.name.value, 1000*(update.end - update.start)), debug_font_color, debug_font_size);
        draw_text(dc, vec2(screen_left, screen_top-debug_font_size*3), string_pushf(frame_arena, "%s: %0.02fms", render.name.value, 1000*(render.end - render.start)), debug_font_color, debug_font_size);
        draw_text(dc, vec2(screen_left, screen_top-debug_font_size*4), string_pushf(frame_arena, "Mouse: [%0.02f, %0.02f]", mouse.world.x, mouse.world.y), debug_font_color, debug_font_size);
        draw_text(dc, vec2(screen_left, screen_top-debug_font_size*5), string_pushf(frame_arena, "MouseButtonState: 0x%x", mouse.button_state), debug_font_color, debug_font_size);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}