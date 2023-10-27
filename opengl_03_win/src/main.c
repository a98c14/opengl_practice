
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// #include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

#include "opus.h"
#include "opus.c"

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "[ERROR] %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
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

    String fragment_shader = file_read_all_as_string(persistent_arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.frag"));
    String vertex_shader = file_read_all_as_string(persistent_arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.vert"));
    uint32 program = material_new(vertex_shader, fragment_shader);
    mvp_location = glGetUniformLocation(program, "mvp");

    Geometry geom = geometry_triangle_create();

    // RendererConfiguration config = {0};
    // Renderer* renderer = renderer_new(arena, &config);
    // Camera* camera = camera_new(arena);
    // renderer_set_camera(camera);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        Mat4 model = mat4_identity();
        model = mat4_rotate_z(model, 16*(float)glfwGetTime());

        Mat4 projection = mat4_ortho(2 * aspect, 2, 1, -1);
        Mat4 mvp = mul_mat4(projection, model);

        // draw_line(renderer, ViewTypeWorld, 10, 20);
        // draw_triangle(renderer, ViewTypeWorld, 10, 20);
        // draw_quad(renderer, ViewTypeWorld, 30, 20, 20, 20);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp.v);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}