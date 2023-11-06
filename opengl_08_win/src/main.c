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
    Arena* persistent_arena = make_arena_reserve(mb(256));
    Arena* frame_arena = make_arena_reserve(mb(128));
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
        /* frame: init */
        arena_reset(frame_arena);
        time = engine_get_time(time);
        InputMouse mouse = input_mouse_get(window, renderer->camera);

        /* frame: update */
        draw_circle(dc, vec2_zero(), 10, ColorWhite);
        draw_circle_filled(dc, circle_pos, 10, ColorWhite);
        circle_pos = lerp_vec2(circle_pos, mouse.world, time.dt * 8.0f);
        draw_text(dc, vec2(0, -20), string_pushf(frame_arena, "Sample Text. FPS: %d", (int)(1 / time.dt)), ColorWhite);

        /* frame: render */
        renderer_render(renderer, time.dt);
        window_update(window);
    }

    window_destroy(window);
    logger_flush();
    return 0;
}