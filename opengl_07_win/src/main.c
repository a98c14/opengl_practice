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
    /* Initialization */
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

    /* Main Loop */
    while (!glfwWindowShouldClose(window->glfw_window))
    {
        arena_reset(frame_arena);
        time = engine_get_time(time);
        InputMouse mouse = input_mouse_get(window, renderer->camera);
        draw_circle(dc, vec2_zero(), 10, ColorWhite);
        draw_circle_filled(dc, vec2(20, 0), 10, ColorWhite);

        renderer_render(renderer, time.dt);
        window_update(window);
    }

    window_destroy(window);
    return 0;
}