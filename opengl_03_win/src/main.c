
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// #include "linmath.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
		case GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = "NOTIFICATION"; break;
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
    Arena* persistent_arena = make_arena_reserve(mb(16));
    Arena* frame_arena = make_arena_reserve(mb(16));

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, 0);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simple example", NULL, NULL);
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
    glViewport(0, 0, width, height);
    float32 aspect = width / (float)height;

    Geometry geom = geometry_quad_create();

    float32 world_height = 100;
    float32 world_width = world_height * aspect;
    Camera camera = camera_new(world_width, world_height, 1, -1, WINDOW_WIDTH, WINDOW_HEIGHT);
    DrawContext* dc = draw_context_new(persistent_arena, &camera);

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glEnable(GL_ALPHA_TEST);
    glClearColor(12 / 255.0f, 11 / 255.0f, 20 / 255.0f, 1.0f);

    uint32 boid_count = 350;
    Vec2* positions = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* directions = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* alignment_vectors = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* avoidance_vectors = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* cohesion_vectors = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    int32 scouts[] = { 0, 4, 13, 17, 46, 2, 15 };

    // init
    srand(time(NULL));
    for(int i = 0; i < boid_count; i++)
    {
        float32 x = ((rand() % 10000) / 10000.0f) * 2 - 1;
        float32 y = ((rand() % 10000) / 10000.0f) * 2 - 1;
        positions[i] = mul_vec2_f32(vec2(x, y), 30);
        directions[i] = vec2_right();
    }

    float32 time = (float32)glfwGetTime();
    float32 last_frame_time, dt;

    float32 close_range = 2;
    float32 visual_range = 7;
    float32 avoid_factor = 2;
    float32 alignment_factor = 1.0;
    float32 cohesion_factor = 0.5;
    float32 min_speed = 3;
    float32 max_speed = 15;

    float32 padding = 10;
    float32 bounds_left = -world_width/2+padding;
    float32 bounds_right = world_width/2-padding;
    float32 bounds_bottom =  -world_height/2+padding;
    float32 bounds_top =  world_height/2-padding;

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
        Vec2 mouse_world = mouse_world_position(mouse_raw, camera);

        for(int i = 0; i < boid_count; i++)
        {
            avoidance_vectors[i] = vec2_zero();
            cohesion_vectors[i] = vec2_zero();
            alignment_vectors[i] = vec2_zero();
            Vec2 group_center = vec2_zero();
            int32 neighbour_count = 0;
            for(int j = 0; j < boid_count; j++)
            {
                if(i == j) continue;
                float32 dist = dist_vec2(positions[i], positions[j]);
                if(dist < close_range)
                {
                    Vec2 v = sub_vec2(positions[i], positions[j]);
                    avoidance_vectors[i] = add_vec2(avoidance_vectors[i], v);
                } 
                else if(dist < visual_range)
                {
                    alignment_vectors[i] = add_vec2(alignment_vectors[i], directions[j]);
                    group_center = add_vec2(group_center, positions[j]);
                    neighbour_count++;
                }
            }

            if(neighbour_count > 0)
            {
                alignment_vectors[i] = div_vec2_f32(alignment_vectors[i], neighbour_count);
                cohesion_vectors[i] = sub_vec2(div_vec2_f32(group_center, neighbour_count), positions[i]);
            }
        }

        for(int i = 0; i < boid_count; i++)
        {
            Vec2 direction = directions[i];
            Vec2 avoidance = mul_vec2_f32(avoidance_vectors[i], avoid_factor * dt);
            Vec2 cohesion = mul_vec2_f32(cohesion_vectors[i], cohesion_factor * dt);
            Vec2 alignment = sub_vec2(alignment_vectors[i], direction);
            alignment = mul_vec2_f32(alignment, alignment_factor * dt);

            direction = add_vec2(direction, avoidance);
            direction = add_vec2(direction, cohesion);
            direction = add_vec2(direction, alignment);
            directions[i] = direction;
        }

        for(int i = 0; i < boid_count; i++)
        {
            if(positions[i].x < bounds_left)
                directions[i].x += fabsf((positions[i].x - bounds_left)) * 10 * dt;
             
            if(positions[i].x > bounds_right)
                directions[i].x -= fabsf((positions[i].x - bounds_right)) * 10 * dt;

            if(positions[i].y < bounds_bottom)
                directions[i].y += fabsf((positions[i].y - bounds_bottom)) * 10 * dt;

            if(positions[i].y > bounds_top)
                directions[i].y -= fabsf((positions[i].y - bounds_top)) * 10 * dt;
        }

        // speed limits
        for(int i = 0; i < boid_count; i++)
        {
            float32 speed = len_vec2(directions[i]);
            if(speed < min_speed)
            {
                float32 new_speed = lerp_f32(speed, min_speed, dt * 8);
                directions[i] = mul_vec2_f32(div_vec2_f32(directions[i], speed), new_speed);
            }

            if(speed < max_speed)
            {
                float32 new_speed = lerp_f32(speed, max_speed, dt * 8);
                directions[i] = mul_vec2_f32(div_vec2_f32(directions[i], speed), new_speed);
            }
        }

        for(int i = 0; i < countof(scouts); i++)
        {
            int32 boid_index = scouts[i];
            Vec2 pos = positions[boid_index];
            Vec2 mouse_direction = mul_vec2_f32(norm_vec2(sub_vec2(mouse_world, pos)), dt * 10);
            directions[boid_index] = add_vec2(directions[boid_index], mouse_direction);
        }

        // update positions
        for(int i = 0; i < boid_count; i++)
        {
            positions[i] = add_vec2(positions[i], mul_vec2_f32(directions[i], dt));
        }

        draw_bounds(dc, -world_width/2+padding, world_width/2-padding, -world_height/2+padding, world_height/2-padding);

        for(int i = 0; i < boid_count; i++)
        {
            Vec2 pos = positions[i];
            Vec2 dir = directions[i];
            draw_boid(dc, pos, dir, 0.5);

            // draw debug stuff for the first boid only
            if(i == 0)
            {
                draw_line(dc, pos, mouse_world);
                draw_circle(dc, pos, close_range);
                draw_circle(dc, pos, visual_range);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}