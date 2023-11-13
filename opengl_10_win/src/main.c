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
    Bounds screen = { .left = -renderer->camera.world_width / 2, .right = renderer->camera.world_width / 2, .top = renderer->camera.world_height / 2, .bottom = -renderer->camera.world_height / 2};
    UIContext* ctx = ui_context_new(persistent_arena, dc, default_theme);    

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
        float32 size = default_theme->font_window_header.font_size;

        StyleRect test_style = default_theme->rect_default;
        test_style.color = color_to_vec4(ColorRed600);
        test_style.border_radius = default_theme->rounded_none;
        test_style.softness = 1;

        StyleRect header_style = default_theme->rect_default;
        header_style.color = color_to_vec4(ColorRed900);
        header_style.border_radius = (BorderRadius){ .bl = 0, .br = 0, .tr = 2, .tl = 2 };

        draw_line(dc, vec2(-200, 0), vec2(200, 0), ColorRed900, 2);
        draw_line(dc, vec2(0, -200), vec2(0, 200), ColorRed900, 2);

        draw_rect(ctx->dc, rect(0, 100, 100, 100), 0, 1, ctx->theme->rect_debug);
        draw_rect(ctx->dc, rect(-200, 100, 50, 50), 0, 1, ctx->theme->rect_debug);
        
        ui_frame_begin(ctx, vec2(5, -5), vec2(100, 0), AlignmentTopLeft, vec2(2, 2));
            ui_text(ctx, string("First Line"));
            ui_text(ctx, string("Second Line"));
            ui_text(ctx, string("Third Line"));
        ui_frame_end(ctx);

        profiler_end(&update);
        /* frame: render */
        Profiler render = profiler_begin(string("Render"));
        renderer_render(renderer, time.dt);
        window_update(window);
        profiler_end(&render);
        profiler_end(&main_frame);
        arena_reset(frame_arena);
        
        float32 font_size = default_theme->font_debug.font_size;
        draw_text(dc, vec2(screen.left, screen.top), string_pushf(frame_arena, "%s: %0.02fms", main_frame.name.value, 1000*(main_frame.end - main_frame.start)), AlignmentTopLeft, default_theme->font_debug);
        draw_text(dc, vec2(screen.left, screen.top-font_size), string_pushf(frame_arena, "%s: %0.02fms", update.name.value, 1000*(update.end - update.start)), AlignmentTopLeft, default_theme->font_debug);
        draw_text(dc, vec2(screen.left, screen.top-font_size*2), string_pushf(frame_arena, "%s: %0.02fms", render.name.value, 1000*(render.end - render.start)), AlignmentTopLeft, default_theme->font_debug);
        draw_text(dc, vec2(screen.left, screen.top-font_size*3), string_pushf(frame_arena, "Mouse: [%0.02f, %0.02f]", mouse.world.x, mouse.world.y), AlignmentTopLeft, default_theme->font_debug);
        draw_text(dc, vec2(screen.left, screen.top-font_size*4), string_pushf(frame_arena, "MouseButtonState: 0x%x", mouse.button_state), AlignmentTopLeft, default_theme->font_debug);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}