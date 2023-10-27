
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
    { .x=-0.2268,   .y= -0.067f  },
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

int main(void)
{
    Arena* persistent_arena = make_arena_reserve(mb(16));
    Arena* frame_arena = make_arena_reserve(mb(16));

    GLint mvp_location;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, 0);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);
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

    // Renderer* renderer = renderer_new(window);

    int32 width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float32 aspect = width / (float)height;
    glViewport(0, 0, width, height);
    Mat4 projection = mat4_ortho(10 * aspect, 10, 1, -1);

    String fragment_shader = file_read_all_as_string(persistent_arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.frag"));
    String vertex_shader = file_read_all_as_string(persistent_arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.vert"));
    uint32 program = material_new(vertex_shader, fragment_shader);
    mvp_location = glGetUniformLocation(program, "mvp");

    Geometry geom = geometry_quad_create();
    // Geometry geom = geometry_triangle_create();

    // RendererConfiguration config = {0};
    // Renderer* renderer = renderer_new(arena, &config);
    // Camera* camera = camera_new(arena);
    // renderer_set_camera(camera);

    while (!glfwWindowShouldClose(window))
    {
        float32 time = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT);

        // draw_line(renderer, ViewTypeWorld, 10, 20);
        // draw_triangle(renderer, ViewTypeWorld, 10, 20);
        // draw_quad(renderer, ViewTypeWorld, 30, 20, 20, 20);
        glUseProgram(program);
        for(int i = 0; i < 50; i++)
        {
            float32 position_factor = sinf(time);
            float32 scale_factor = cosf(time);

            Vec2 pos = starting_positions[i];
            Mat4 translation = mat4_translation(vec3(pos.x * 3, pos.y * 3, 0));
            Mat4 rotation = mat4_rotation(16.0f * time * i);
            Mat4 scale = mat4_scale(vec3(scale_factor, scale_factor, 0));
            Mat4 transform = mat4_transform(translation, rotation, scale);
            Mat4 mvp = mul_mat4(projection, transform);

            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp.v);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}