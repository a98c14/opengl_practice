
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// #include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

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

global const Vec2 starting_positions[] =
{
    { .x=-0.0,      .y= -0.0  },
    { .x=-0.9521,   .y= -0.4604f },
    { .x=0,         .y= 0.7251f  },
    { .x=-0.1799,   .y= -0.2773f },
    { .x=0.1564,    .y= 0.0417f  },
    { .x=-0.8926,   .y= 0.7107f  },
    { .x=-0.9266,   .y= 0.7657f  },
    { .x=-0.6868,   .y= -0.5848f },
    { .x=-0.3643,   .y= 0.3128f  },
    { .x=-0.2575,   .y= -0.1681f },
    { .x=-0.4415,   .y= -0.2265f },
    { .x=-0.9487,   .y= 0.1192f  },
    { .x=-0.785,    .y= 0.7981f  },
    { .x=-0.4931,   .y= 0.9602f  },
    { .x=0.6157,    .y= -0.3377f },
    { .x=-0.9047,   .y= -0.3919f },
    { .x=0.7594,    .y= 0.212f   },
    { .x=-0.4572,   .y= -0.0128f },
    { .x=0.5657,    .y= 0.4076f  },
    { .x=-0.9804,   .y= 0.4437f  },
    { .x=0.7297,    .y= 0.877f   },
    { .x=-0.6937,   .y= -0.1672f },
    { .x=0.2351,    .y= -0.7223f },
    { .x=-0.5766,   .y= -0.4238f },
    { .x=0.0133,    .y= -0.7941f },
    { .x=0.4692,    .y= 0.3434f  },
    { .x=0.9087,    .y= 0.5143f  },
    { .x=-0.0282,   .y= -0.8889f },
    { .x=-0.4234,   .y= 0.3741f  },
    { .x=0.3301,    .y= 0.058f   },
    { .x=-0.8427,   .y= 0.6048f  },
    { .x=-0.0451,   .y= 0.0031f  },
    { .x=-0.0092,   .y= 0.8157f  },
    { .x=0.7804,    .y= 0.6021f  },
    { .x=-0.5375,   .y= 0.6156f  },
    { .x=0.4278,    .y= 0.9165f  },
    { .x=-0.8019,   .y= -0.3707f },
    { .x=-0.7688,   .y= 0.9735f  },
    { .x=0.9868,    .y= 0.6414f  },
    { .x=0.0887,    .y= -0.0502f },
    { .x=0.0329,    .y= -0.6696f },
    { .x=0.2576,    .y= -0.9658f },
    { .x=0.7574,    .y= -0.2227f },
    { .x=-0.8146,   .y= 0.8492f  },
    { .x=0.0033,    .y= -0.3412f },
    { .x=0.0392,    .y= -0.0378f },
    { .x=-0.1185,   .y= 0.5081f  },
    { .x=-0.9105,   .y= -0.7117f },
    { .x=0.8809,    .y= 0.2149f  },
    { .x=-0.3786,   .y= -0.9675f }
};

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

    uint32 boid_count = 1;
    Vec2* positions = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    Vec2* directions = arena_push_array_zero(persistent_arena, Vec2, boid_count);
    float32* speeds = arena_push_array_zero(persistent_arena, float32, boid_count);

    // init
    for(int i = 0; i < boid_count; i++)
    {
        positions[i] = starting_positions[i];
        speeds[i] = 5;
        directions[i] = vec2_right();
    }

    float32 time = (float32)glfwGetTime();
    float32 last_frame_time, dt;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

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
            positions[i] = add_vec2(positions[i], mul_vec2_f32(directions[i], speeds[i] * dt));
        }

        for(int i = 0; i < boid_count; i++)
        {
            directions[i] = norm_vec2(sub_vec2(mouse_world, positions[i]));
        }

        for(int i = 0; i < boid_count; i++)
        {
            Vec2 pos = positions[i];
            Vec2 dir = directions[i];
            draw_line(dc, pos, mouse_world);
            draw_boid(dc, pos, dir, 1);
            draw_circle(dc, pos, 20);
            draw_circle(dc, pos, 50);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}