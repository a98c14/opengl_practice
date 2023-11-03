#include "boids.h"
#include "boids.c"

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

internal void
message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	char* source_str;
    switch (source)
    {
		case GL_DEBUG_SOURCE_API: source_str = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source_str = "WINDOW SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "SHADER COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: source_str = "THIRD PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION: source_str = "APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER: source_str = "OTHER"; break;
        default: source_str = "NO_SOURCE"; break;
    }

	char* type_str;
    switch (type)
    {
		case GL_DEBUG_TYPE_ERROR: type_str = "ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "DEPRECATED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "UNDEFINED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_PORTABILITY: type_str = "PORTABILITY"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: type_str = "PERFORMANCE"; break;
		case GL_DEBUG_TYPE_MARKER: type_str = "MARKER"; break;
		case GL_DEBUG_TYPE_OTHER: type_str = "OTHER"; break;
        default: type_str = "NO_TYPE"; break;
    }

	char* severity_str;
    switch (severity)
    {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return; // severity_str = "NOTIFICATION"; break;
		case GL_DEBUG_SEVERITY_LOW: severity_str = "LOW"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severity_str = "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_HIGH: severity_str = "HIGH"; break;
        default: severity_str = "NO_SEVERITY"; break;
    }

	printf("[GL_%s][%s|%s] %s\n", severity_str, source_str, type_str, message);
}

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main(void)
{
    Arena* persistent_arena = make_arena_reserve(mb(128));
    Arena* frame_arena = make_arena_reserve(mb(16));

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
    glfwSwapInterval(1);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, 0);

    int32 width, height;
    glfwGetFramebufferSize(window, &width, &height);

    RendererConfiguration renderer_configuration = {
        .window_width = width,
        .window_height = height,
        .world_height = 100,
        .clear_color = ColorWhite
    };
    Renderer* renderer = renderer_new(persistent_arena, &renderer_configuration);

    DrawContext* dc = draw_context_new(persistent_arena, renderer);

    float32 time = (float32)glfwGetTime();
    float32 last_frame_time, dt;
    Geometry geometry = geometry_quad_create();

    float32 padding = 10;
    float32 bounds_left = -renderer->camera.world_width/2+padding;
    float32 bounds_right = renderer->camera.world_width/2-padding;
    float32 bounds_bottom = -renderer->camera.world_height/2+padding;
    float32 bounds_top = renderer->camera.world_height/2-padding;

    float32 close_range = 2;
    float32 visual_range = 7;
    float32 avoid_factor = 2;
    float32 alignment_factor = 1.0;
    float32 cohesion_factor = 0.5;
    float32 min_speed = 3;
    float32 max_speed = 15;

    uint32 boid_count = 1;
    Vec2* positions = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* directions = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* alignment_vectors = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* avoidance_vectors = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* cohesion_vectors = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    int32 scouts[] = { 0, 4, 13, 17, 46, 2, 15 };

    BoidBucketHashMap* hash_map = boid_bucket_new(persistent_arena, renderer->camera.world_width, renderer->camera.world_height, visual_range);
    

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        arena_reset(frame_arena);

        /* timers */
        last_frame_time = time;
        time = (float32)glfwGetTime();
        dt = time - last_frame_time;

        /* input */
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        Vec2 mouse_raw = vec2(xpos, ypos);
        Vec2 mouse_world = mouse_world_position(mouse_raw, renderer->camera);

        draw_bounds(dc, bounds_left, bounds_right, bounds_bottom, bounds_top, ColorBlack);

        // fill buckets 
        for(int i = 0; i < boid_count; i++)
        {
            insert_to_bucket(hash_map, positions[i], i);
        }

        // color buckets
        for(int i = 0; i < boid_count; i++)
        {
            BoidBucket* bucket = get_bucket(hash_map, positions[i]);
            draw_text(dc, vec2(-20, -20), string_pushf(frame_arena, "X: %d, Y: %d", bucket->x, bucket->y));
            float32 x = bucket->x * hash_map->cell_size;
            float32 y = bucket->y * hash_map->cell_size;
            draw_bounds(dc, x, x+visual_range, y, y+visual_range, ColorGreenPastel);
        }

        // draw boids
        for(int i = 0; i < boid_count; i++)
        {
            draw_boid(dc, positions[i], vec2(5, 2), 2, ColorBlack);
            draw_circle(dc, positions[i], 10, ColorBlack);
        }

        for(int i = 0; i < boid_count; i++)
        {
            positions[i].x += 5 * dt;
            positions[i].y += 2 * dt;
        }

        // String str = string("!\"#$%%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
        // draw_text(dc, vec2(-100, 0), str);
        // String fps_str = string_pushf(frame_arena, "Frame: %.4f ms", dt * 1000);
        // draw_text(dc, vec2(cosf(time * 4), -10 + sinf(time * 10)), fps_str);
        renderer_render(renderer, dt);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}