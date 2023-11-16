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
        
        if(cache_clock > cache_rate)
        {
            profiler_refresh_cache(main_frame);
            profiler_refresh_cache(update);
            profiler_refresh_cache(render);
            cache_clock = 0;
        }
        cache_clock += time.dt;

        int32 row_count = 6;
        int32 row_height = 10;

        /* timing info */
        Rect timing_info_container = ui_container(ctx, rect_anchor(rect(4, 4, 200, row_height * row_count), screen, ANCHOR_BL_BL), default_theme->container_default);
        LayoutGrid layout = layout_grid(timing_info_container, 4, row_count, vec2(4, 4));
        ui_label(ctx, layout_grid_cell(layout, 1, 0), string("Avg"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 2, 0), string("Min"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 3, 0), string("Max"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 0, 1), string("Time:"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 2, 1), string("-"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 3, 1), string("-"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 1), string_pushf(frame_arena, "%0.2fs", time.current_frame / 1000), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 0, 2), string("FPS:"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 2), string_pushf(frame_arena, "%d", (int32)(1000 / main_frame->cached_avg)), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 2, 2), string_pushf(frame_arena, "%d", (int32)(1000 / main_frame->cached_max)), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 3, 2), string_pushf(frame_arena, "%d", (int32)(1000 / main_frame->cached_min)), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 0, 3), string_pushf(frame_arena, "%s:", main_frame->name.value), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 3), string_pushf(frame_arena, "%0.02fms", main_frame->cached_avg), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 2, 3), string_pushf(frame_arena, "%0.02fms", main_frame->cached_min), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 3, 3), string_pushf(frame_arena, "%0.02fms", main_frame->cached_max), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 0, 4), string_pushf(frame_arena, "%s:", update->name.value), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 4), string_pushf(frame_arena, "%0.02fms", update->cached_avg), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 2, 4), string_pushf(frame_arena, "%0.02fms", update->cached_min), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 3, 4), string_pushf(frame_arena, "%0.02fms", update->cached_max), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 0, 5), string_pushf(frame_arena, "%s:", render->name.value), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(layout, 1, 5), string_pushf(frame_arena, "%0.02fms", render->cached_avg), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 2, 5), string_pushf(frame_arena, "%0.02fms", render->cached_min), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(layout, 3, 5), string_pushf(frame_arena, "%0.02fms", render->cached_max), default_theme->label_default);
        
        /* input info */
        Rect input_info_container = ui_container(ctx, rect_anchor(rect(2, 0, 120, row_height * row_count), timing_info_container, ANCHOR_TR_TL), default_theme->container_default);
        LayoutGrid input_layout = layout_grid(input_info_container, 2, row_count, vec2(4, 4));
        ui_label(ctx, layout_grid_cell(input_layout, 0, 1), string("Mouse:"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(input_layout, 1, 1), string_pushf(frame_arena, "[%0.2f, %0.2f]", mouse.world.x, mouse.world.y), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(input_layout, 0, 2), string("MouseButton:"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(input_layout, 1, 2), string_pushf(frame_arena, "0x%x", mouse.button_state), default_theme->label_default);

        /* memory info */
        Rect memory_info_container = ui_container(ctx, rect_anchor(rect(2, 0, 200, row_height * row_count), input_info_container, ANCHOR_TR_TL), default_theme->container_default);
        LayoutGrid memory_layout = layout_grid(memory_info_container, 4, row_count, vec2(4, 4));
        ui_label(ctx, layout_grid_cell(memory_layout, 0, 0), string("Memory"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(memory_layout, 1, 0), string("Used"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(memory_layout, 2, 0), string("Available"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(memory_layout, 3, 0), string("Ratio"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(memory_layout, 0, 1), string("Persistent"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(memory_layout, 1, 1), string_pushf(frame_arena, "%0.2fmb", (float32)to_mb(persistent_arena->pos)), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(memory_layout, 2, 1), string_pushf(frame_arena, "%0.2fmb", (float32)to_mb(persistent_arena->cap)), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(memory_layout, 3, 1), string_pushf(frame_arena, "%0.2f%%", 100 * (float32)to_mb(persistent_arena->pos) / (float32)to_mb(persistent_arena->cap)), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(memory_layout, 0, 2), string("Frame"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(memory_layout, 1, 2), string_pushf(frame_arena, "%0.2fmb", (float32)to_mb(frame_arena->pos)), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(memory_layout, 2, 2), string_pushf(frame_arena, "%0.2fmb", (float32)to_mb(frame_arena->cap)), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(memory_layout, 3, 2), string_pushf(frame_arena, "%0.2f%%", 100 * (float32)to_mb(frame_arena->pos) / (float32)to_mb(frame_arena->cap)), default_theme->label_default);

        /* renderer info */
        Rect renderer_info_container = ui_container(ctx, rect_anchor(rect(2, 0, 200, row_height * row_count), memory_info_container, ANCHOR_TR_TL), default_theme->container_default);
        LayoutGrid renderer_layout = layout_grid(renderer_info_container, 4, row_count, vec2(4, 4));
        ui_label(ctx, layout_grid_cell(renderer_layout, 0, 0), string("Renderer"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(renderer_layout, 0, 1), string("Draw Count"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(renderer_layout, 1, 1), string_pushf(frame_arena, "%d", renderer->stat_draw_count), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(renderer_layout, 0, 2), string("Object Count"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(renderer_layout, 1, 2), string_pushf(frame_arena, "%d", renderer->stat_object_count), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(renderer_layout, 0, 3), string("Buffer Count"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(renderer_layout, 1, 3), string_pushf(frame_arena, "%d / %d", renderer->stat_initialized_buffer_count, MATERIAL_DRAW_BUFFER_CAPACITY), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(renderer_layout, 2, 1), string("Avg. Probe"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(renderer_layout, 3, 1), string_pushf(frame_arena, "%0.2f", renderer->stat_probe_count_sum / (float32)renderer->stat_probe_count), default_theme->label_default);
        ui_label(ctx, layout_grid_cell(renderer_layout, 2, 2), string("Max Probe"), default_theme->label_bold);
        ui_label(ctx, layout_grid_cell(renderer_layout, 3, 2), string_pushf(frame_arena, "%d", renderer->stat_probe_count_max), default_theme->label_default);
        arena_reset(frame_arena);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}