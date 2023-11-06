#include "opus.h"
#include "opus.c"

internal void
error_callback(int error, const char* description)
{
    fprintf(stderr, "[ERROR] %s\n", description);
}

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
    Arena* persistent_arena = make_arena_reserve(mb(256));
    Arena* frame_arena = make_arena_reserve(mb(128));
    log_info("Initialized arenas");

    if(GL_ARB_direct_state_access)
    {
        printf("We have direct state access\n");
    }

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 0);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simple Example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(0);
    glEnable(GL_DEBUG_OUTPUT);

    int32 width, height;
    glfwGetFramebufferSize(window, &width, &height);

    RendererConfiguration renderer_configuration = {
        .window_width = width,
        .window_height = height,
        .world_height = 100,
        .clear_color = ColorWhite
    };
    Renderer* renderer = renderer_new(persistent_arena, &renderer_configuration);
    renderer_enable_debug();
    
    DrawContext* dc = draw_context_new(persistent_arena, renderer);

    float32 frame_time = (float32)glfwGetTime();
    float32 last_frame_time, dt;
    Geometry geometry = geometry_quad_create();

    srand(time(NULL));
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        arena_reset(frame_arena);

        /* timers */
        last_frame_time = frame_time;
        frame_time = (float32)glfwGetTime();
        dt = frame_time - last_frame_time;
        
        /* input */
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        Vec2 mouse_raw = vec2(xpos, ypos);
        Vec2 mouse_world = mouse_world_position(mouse_raw, renderer->camera);


        renderer_render(renderer, dt);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}