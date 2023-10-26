
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

#include "opus.h"
#include "opus.c"

static const float32 vertices[] =
{
    // positions,        // colors
    -0.6f, -0.4f, 0.0f,  1.f, 0.f, 0.f,
     0.6f, -0.4f, 0.0f,  0.f, 1.f, 0.f,
      0.f,  0.6f, 0.0f,  0.f, 0.f, 1.f
};

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
	// auto const src_str = [source]() {
	// 	switch (source)
	// 	{
	// 	case GL_DEBUG_SOURCE_API: return "API";
	// 	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
	// 	case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
	// 	case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
	// 	case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
	// 	case GL_DEBUG_SOURCE_OTHER: return "OTHER";
	// 	}
	// }();

	// auto const type_str = [type]() {
	// 	switch (type)
	// 	{
	// 	case GL_DEBUG_TYPE_ERROR: return "ERROR";
	// 	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
	// 	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
	// 	case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
	// 	case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
	// 	case GL_DEBUG_TYPE_MARKER: return "MARKER";
	// 	case GL_DEBUG_TYPE_OTHER: return "OTHER";
	// 	}
	// }();

	// auto const severity_str = [severity]() {
	// 	switch (severity) {
	// 	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	// 	case GL_DEBUG_SEVERITY_LOW: return "LOW";
	// 	case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
	// 	case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
	// 	}
	// }();
	printf("%s\n", message);
}

int main(void)
{
    Arena* persistent_arena = make_arena_reserve(mb(16));
    Arena* frame_arena = make_arena_reserve(mb(16));

    GLint mvp_location, vpos_location, vcol_location;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
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
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}