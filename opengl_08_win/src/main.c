#include <opus.h>
#include <opus.c>

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
        .world_height = 100,
        .clear_color = ColorSlate900
    };
    
    Renderer* renderer = renderer_new(persistent_arena, &renderer_configuration);
    DrawContext* dc = draw_context_new(persistent_arena, renderer);
    EngineTime time = engine_time_new();

    /* application state */
    Vec2 circle_pos = vec2_zero();

    /* main loop */
    while (!glfwWindowShouldClose(window->glfw_window))
    {
        Profiler main_frame = profiler_begin(string("MainFrame"));
        /* frame: init */
        time = engine_get_time(time);
        InputMouse mouse = input_mouse_get(window, renderer->camera);

        /* frame: update */
        Profiler update = profiler_begin(string("Update"));
        int32 min_y = -100;
        int32 max_y = 100;
        int32 min_x = -200;
        int32 max_x = 200;
        uint32 instance_count = (max_x - min_x)*(max_y - min_y);

        DrawBufferArray* draw_buffer_array = renderer_buffer_request_batched(frame_arena, dc->renderer, FRAME_BUFFER_INDEX_DEFAULT, dc->material_circle_instanced, ViewTypeWorld, TEXTURE_INDEX_NULL, instance_count);
        for(int y = min_y; y < max_y; y++)
        {
            for(int x = min_x; x < max_x; x++)
            {
                Mat4 model = transform_quad_aligned(vec2(x / 4.0f, y / 4.0f), vec2(0.4f, 0.4f));
                ShaderDataCircle uniform_data = {
                    .color = vec4(0.5, (float32)y / (max_y - min_y), (float32)x / (max_x - min_x), 1),
                    .fill_ratio = 1,
                };
                draw_buffer_array_insert(draw_buffer_array, model, &uniform_data);
            }
        }
        circle_pos = lerp_vec2(circle_pos, mouse.world, time.dt * 8.0f);
        profiler_end(&update);

        /* frame: render */
        Profiler render = profiler_begin(string("Render"));
        renderer_render(renderer, time.dt);
        window_update(window);
        profiler_end(&render);
        profiler_end(&main_frame);
        arena_reset(frame_arena);

        float32 screen_left = -renderer->camera.world_width / 2;
        float32 screen_top = renderer->camera.world_height / 2 - 2;
        draw_text(dc, vec2(screen_left, screen_top), string_pushf(frame_arena, "%s: %0.02fms", main_frame.name.value, 1000*(main_frame.end - main_frame.start)), ColorWhite);
        draw_text(dc, vec2(screen_left, screen_top-2), string_pushf(frame_arena, "%s: %0.02fms", update.name.value, 1000*(update.end - update.start)), ColorWhite);
        draw_text(dc, vec2(screen_left, screen_top-4), string_pushf(frame_arena, "%s: %0.02fms", render.name.value, 1000*(render.end - render.start)), ColorWhite);
        draw_text(dc, vec2(screen_left, screen_top-6), string_pushf(frame_arena, "InstanceCount: %d", instance_count), ColorWhite);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}