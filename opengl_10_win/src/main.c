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
    Bounds screen = { .left = -renderer->camera.world_width / 2, .right = renderer->camera.world_width / 2, .top = renderer->camera.world_height / 2, .bottom = -renderer->camera.world_height / 2};

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
        test_style.color = color_to_vec4(ColorDebugWindow);
        test_style.border_radius = default_theme->rounded_none;

        StyleRect header_style = default_theme->rect_default;
        header_style.color = color_to_vec4(ColorRed900);
        header_style.border_radius = (BorderRadius){ .bl = 0, .br = 0, .tr = 2, .tl = 2 };

        float32 padding_y = 2;
        float32 padding_x = 2;
        float32 window_width = 200;
        float32 window_height = 100;
        float32 header_height = size + padding_y * 2;

        Rect frame = rect_aligned(0, 0, window_width, window_height - header_height, RectAlignmentTypeTopLeft);
        Rect header = rect_aligned(0, 0, window_width, header_height, RectAlignmentTypeBottomLeft);
        header = rect_anchor(frame, RectAlignmentTypeTopLeft, header);
        draw_rect(dc, frame, 0, 0, test_style);
        draw_text(dc, add_vec2(rect_bl(header), vec2(padding_x, padding_y)), string("Header"), RectAlignmentTypeBottomLeft, default_theme->font_window_header);
        
        Rect input_text_bounds = draw_text(dc, add_vec2(rect_tl(frame), vec2(padding_x, -padding_y)), string("Input:"), RectAlignmentTypeTopLeft, default_theme->font_window_header);
        Rect input = rect_aligned(0, -1, window_width - input_text_bounds.w - padding_x - padding_x, input_text_bounds.h, RectAlignmentTypeBottomLeft);
        input = rect_anchor(input_text_bounds, RectAlignmentTypeBottomRight, input);
        draw_rect(dc, input, 0, 0, default_theme->rect_default);
        

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

        Rect info_frame = rect_aligned(screen.left, screen.bottom, screen.right - screen.left, window_height, RectAlignmentTypeBottomLeft);
        draw_rect(dc, info_frame, 0, 0, test_style);
        float32 font_size = default_theme->font_debug.font_size;
        draw_text(dc, vec2(screen.left, screen.top), string_pushf(frame_arena, "%s: %0.02fms", main_frame.name.value, 1000*(main_frame.end - main_frame.start)), RectAlignmentTypeTopLeft, default_theme->font_debug);
        draw_text(dc, vec2(screen.left, screen.top-font_size), string_pushf(frame_arena, "%s: %0.02fms", update.name.value, 1000*(update.end - update.start)), RectAlignmentTypeTopLeft, default_theme->font_debug);
        draw_text(dc, vec2(screen.left, screen.top-font_size*2), string_pushf(frame_arena, "%s: %0.02fms", render.name.value, 1000*(render.end - render.start)), RectAlignmentTypeTopLeft, default_theme->font_debug);
        draw_text(dc, vec2(screen.left, screen.top-font_size*3), string_pushf(frame_arena, "Mouse: [%0.02f, %0.02f]", mouse.world.x, mouse.world.y), RectAlignmentTypeTopLeft, default_theme->font_debug);
        draw_text(dc, vec2(screen.left, screen.top-font_size*4), string_pushf(frame_arena, "MouseButtonState: 0x%x", mouse.button_state), RectAlignmentTypeTopLeft, default_theme->font_debug);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}