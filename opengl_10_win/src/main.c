#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib,"user32.lib")

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
    Rect screen = rect_wh(renderer->world_width, renderer->world_height);
    UIContext* ctx = ui_context_new(persistent_arena, frame_arena, dc, default_theme);

    /* application state */
    // ShowCursor(0);

    Vec2 window_pos = vec2_zero();
    bool32 window_is_enabled = 1;
    float32 slider_value = 4;
    InputMouse mouse = {0};
    /* main loop */
    while (!glfwWindowShouldClose(window->glfw_window))
    {
        Profiler main_frame = profiler_begin(string("MainFrame"));
        /* frame: init */
        time = engine_get_time(time);
        mouse = input_mouse_get(window, renderer->camera, mouse);
        ctx->mouse = mouse;
        ctx->time = time;

        /* frame: update */
        Profiler update = profiler_begin(string("Update"));
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

        profiler_end(&update);
        /* frame: render */
        Profiler render = profiler_begin(string("Render"));
        renderer_render(renderer, time.dt);
        window_update(window);
        profiler_end(&render);
        profiler_end(&main_frame);
        arena_reset(frame_arena);

        Rect settings_panel = draw_rect(dc, rect_anchor(rect_wh(screen.w, 100), screen, ANCHOR_BL_BL), 0, SORT_LAYER_INDEX_DEFAULT, default_theme->rect_default);
        Rect first = draw_rect(dc, rect_anchor(rect_wh(100, 20), settings_panel, ANCHOR_TL_TL), 0, SORT_LAYER_INDEX_DEFAULT+1, default_theme->rect_debug);
        Rect second = draw_rect(dc, rect_anchor(rect_wh(100, 20), first, ANCHOR_TR_TL), 0, SORT_LAYER_INDEX_DEFAULT+1, default_theme->rect_debug);
        Rect third = draw_rect(dc, rect_anchor(rect_wh(100, 20), second, ANCHOR_BL_TL), 0, SORT_LAYER_INDEX_DEFAULT+1, default_theme->rect_debug);
        Rect fourth = draw_rect(dc, rect_anchor(rect_wh(100, 20), third, ANCHOR_TL_TR), 0, SORT_LAYER_INDEX_DEFAULT+1, default_theme->rect_debug);
        // StyleLabel t = style_label();
        ui_label(ctx, fourth, string("label"), default_theme->label_default);

        float32 font_size = default_theme->font_debug.font_size;
        draw_text(dc, rect_tl(screen), string_pushf(frame_arena, "%s: %0.02fms", main_frame.name.value, 1000*(main_frame.end - main_frame.start)), AlignmentTopLeft, default_theme->font_debug);
        draw_text(dc, add_vec2(rect_tl(screen), vec2(0, -font_size*1)), string_pushf(frame_arena, "Time: %0.2fs", time.current_frame / 1000), AlignmentTopLeft, default_theme->font_debug);
        draw_text(dc, add_vec2(rect_tl(screen), vec2(0, -font_size*2)), string_pushf(frame_arena, "%s: %0.02fms", update.name.value, 1000*(update.end - update.start)), AlignmentTopLeft, default_theme->font_debug);
        draw_text(dc, add_vec2(rect_tl(screen), vec2(0, -font_size*3)), string_pushf(frame_arena, "%s: %0.02fms", render.name.value, 1000*(render.end - render.start)), AlignmentTopLeft, default_theme->font_debug);
        draw_text(dc, add_vec2(rect_tl(screen), vec2(0, -font_size*4)), string_pushf(frame_arena, "Mouse: [%0.02f, %0.02f]", mouse.world.x, mouse.world.y), AlignmentTopLeft, default_theme->font_debug);
        draw_text(dc, add_vec2(rect_tl(screen), vec2(0, -font_size*5)), string_pushf(frame_arena, "MouseButtonState: 0x%x", mouse.button_state), AlignmentTopLeft, default_theme->font_debug);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}