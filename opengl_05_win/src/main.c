#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// #include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "opus.h"
#include "opus.c"

#include "fonts.h"

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
    Arena* persistent_arena = make_arena_reserve(mb(16));
    Arena* frame_arena = make_arena_reserve(mb(16));

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

    // load texture
    TextureIndex font_texture = texture_new_from_file(renderer, string("..\\assets\\open_sans.png"), 0, 1);
    GlyphAtlas* atlas = glyph_atlas_load(
        persistent_arena,
        &FONT_OPEN_SANS_ATLAS_INFO,
        FONT_OPEN_SANS_GLYPHS,
        countof(FONT_OPEN_SANS_GLYPHS),
        font_texture);

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

        ShaderDataText shader_data = {0};
        shader_data.color = color_to_vec4(ColorBlack);
        shader_data.outline_color = color_to_vec4(ColorInvisible);
        shader_data.thickness = 0.50;
        shader_data.softness = 30;
        shader_data.outline_thickness = 0.2;

        String str = string("!\"#$%%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
        DrawBuffer draw_buffer = renderer_buffer_request(renderer, FRAME_BUFFER_INDEX_DEFAULT, dc->material_text, ViewTypeWorld, font_texture, str.length);
        text_calculate_transforms(atlas, str, 3.5, vec2(-100, 0), RectAlignmentTypeBottomLeft, draw_buffer.model_buffer, 0);
        ShaderDataText* shader_data_buffer = (ShaderDataText*)draw_buffer.uniform_data_buffer;
        for(int i = 0; i < str.length; i++)
        {
            Glyph glyph = glyph_get(atlas, str.value[i]);
            shader_data.glyph_bounds = glyph.atlas_bounds.v;
            memcpy(&shader_data_buffer[i], &shader_data, sizeof(ShaderDataText));
        }

        String str2 = string_pushf(frame_arena, "Frame: %.4f ms", dt * 1000);
        DrawBuffer draw_buffer2 = renderer_buffer_request(renderer, FRAME_BUFFER_INDEX_DEFAULT, dc->material_text, ViewTypeWorld, font_texture, str2.length);
        text_calculate_transforms(atlas, str2, 3.5, vec2(cosf(time * 4), -10 + sinf(time * 10)), RectAlignmentTypeBottomLeft, draw_buffer2.model_buffer, 0);
        shader_data_buffer = (ShaderDataText*)draw_buffer2.uniform_data_buffer;
        for(int i = 0; i < str2.length; i++)
        {
            Glyph glyph = glyph_get(atlas, str2.value[i]);
            shader_data.glyph_bounds = glyph.atlas_bounds.v;
            memcpy(&shader_data_buffer[i], &shader_data, sizeof(ShaderDataText));
        }

        renderer_render(renderer, dt);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}