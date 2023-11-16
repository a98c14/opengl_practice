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

        // LayoutGrid layout = layout_grid();
        // layout_grid_get(1, 2, 1, 1);

        // LayoutStack layout = layout_stack(pos, w);
        // layout_stack_get(layout);

        Rect timing_info_container = ui_container(ctx, rect_anchor(rect(4, 4, 200, 80), screen, ANCHOR_BL_BL), default_theme->container_default);
        LayoutGrid layout = layout_grid(timing_info_container, 4, 8, vec2(4, 4));
        // header
        ui_label(ctx, layout_grid_cell(layout, 1, 0), string("Avg"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 2, 0), string("Max"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 3, 0), string("Min"), default_theme->label_bold);
        
        ui_label(ctx, layout_grid_cell(layout, 0, 1), string("Time:"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 1), string_pushf(frame_arena, "%0.2fs", time.current_frame / 1000), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 0, 2), string("FPS:"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 2), string_pushf(frame_arena, "%d", (int32)(1000 / time.dt)), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 0, 3), string_pushf(frame_arena, "%s:", main_frame.name.value), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 3), string_pushf(frame_arena, "%0.02fms", main_frame.end - main_frame.start), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 0, 4), string_pushf(frame_arena, "%s:", update.name.value), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 4), string_pushf(frame_arena, "%0.02fms", update.end - update.start), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 0, 5), string_pushf(frame_arena, "%s:", render.name.value), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 5), string_pushf(frame_arena, "%0.02fms", render.end - render.start), default_theme->label_default);

        Rect input_info_container = ui_container(ctx, rect_anchor(rect(2, 0, 120, 80), timing_info_container, ANCHOR_TR_TL), default_theme->container_default);
        LayoutGrid input_layout = layout_grid(input_info_container, 2, 8, vec2(4, 4));
        ui_label(ctx, layout_grid_cell(input_layout, 0, 1), string("Mouse:"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(input_layout, 1, 1), string_pushf(frame_arena, "[%0.2f, %0.2f]", mouse.world.x, mouse.world.y), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(input_layout, 0, 2), string("MouseButton:"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(input_layout, 1, 2), string_pushf(frame_arena, "0x%x", mouse.button_state), default_theme->label_default);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}