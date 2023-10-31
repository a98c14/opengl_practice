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
    glViewport(0, 0, width, height);
    float32 aspect = width / (float)height;

    Geometry geom = geometry_quad_create();

    float32 world_height = 100;
    float32 world_width = world_height * aspect;
    Camera camera = camera_new(world_width, world_height, 1, -1, WINDOW_WIDTH, WINDOW_HEIGHT);
    DrawContext* dc = draw_context_new(persistent_arena, &camera);    

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    // glClearColor(12 / 255.0f, 11 / 255.0f, 20 / 255.0f, 1.0f);
    glClearColor(255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 1.0f);


    float32 time = (float32)glfwGetTime();
    float32 last_frame_time, dt;
    float32 padding = 20;

    // load texture
    Texture font_texture = texture_load_from_file(string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_04_win\\assets\\open_sans.png"), 0, 1);
    GlyphAtlas* atlas = glyph_atlas_load(
        persistent_arena,
        &FONT_OPEN_SANS_ATLAS_INFO,
        FONT_OPEN_SANS_GLYPHS,
        countof(FONT_OPEN_SANS_GLYPHS),
        font_texture);

    /* Create Global UBO */
    uint32 global_uniform_buffer_id;
    glGenBuffers(1, &global_uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, global_uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GlobalUniformData), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_SLOT_GLOBAL, global_uniform_buffer_id, 0, sizeof(GlobalUniformData));

    /* Create Texture UBO */
    uint32 texture_uniform_buffer_id;
    glGenBuffers(1, &texture_uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, texture_uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(TextureUniformData), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_SLOT_TEXTURE, texture_uniform_buffer_id, 0, sizeof(TextureUniformData));

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

        TextureUniformData texture_data = {0};
        texture_data.size.x = atlas->texture.width;
        texture_data.size.y = atlas->texture.height;
        glBindBuffer(GL_UNIFORM_BUFFER, texture_uniform_buffer_id);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(TextureUniformData), &texture_data);

        glUseProgram(dc->material_text.gl_program_id);
        glBindBuffer(GL_UNIFORM_BUFFER, dc->material_text.uniform_buffer_id);
        glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_SLOT_CUSTOM, dc->material_text.uniform_buffer_id, 0, dc->material_text.uniform_data_size);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(font_texture.gl_texture_type, font_texture.gl_texture_id);

        ShaderDataText shader_data = {0};
        shader_data.color.r = 0;
        shader_data.color.g = 0;
        shader_data.color.b = 0;
        shader_data.color.a = 1;
        shader_data.outline_color.r = 0;
        shader_data.outline_color.g = 0;
        shader_data.outline_color.b = 0;
        shader_data.outline_color.a = 0;
        shader_data.u_thickness = 0.50;
        shader_data.softness = 30;
        shader_data.outline_thickness = 0.1;
        
        Mat4* models = arena_push_array(frame_arena, Mat4, 20);
        String str = string("Testing with the boys 123[];l");
        Rect bounds = text_calculate_transforms(atlas, str, 1.5, vec2_zero(), RectAlignmentTypeBottomLeft, models, 0);
        for(int i = 0; i < str.length; i++)
        {
            Glyph glyph = glyph_get(atlas, str.value[i]);
            shader_data.glyph_bounds = glyph.atlas_bounds.v;
            Mat4 mvp = mat4_mvp(models[i], dc->camera->view, dc->camera->projection);
            glUniformMatrix4fv(dc->material_text.location_model, 1, GL_FALSE, mvp.v);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, dc->material_text.uniform_data_size, &shader_data);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // Vec2 pos = vec2(-world_width/2+5, -world_height/2 + 3);
        // for(int32 x = 0; x < 20; x++)
        // {
        //     if(x == 10)
        //     {
        //         glUseProgram(dc->material_text_v2.gl_program_id);
        //     }
        //     pos.x += 7;
        //     pos.y = -world_height/2 + 3;
        //     for(int32 y = 0; y <= 20; y++)
        //     {
        //         pos.y += 1 + y / 4.0;
                
        //         Vec2 default_size = vec2(glyph.plane_bounds.right - glyph.plane_bounds.left, glyph.plane_bounds.top - glyph.plane_bounds.bottom);
        //         Vec2 scaled_size = mul_vec2_f32(default_size, 0.5 + y/4.0f);

                
        //         Mat4 transform = transform_quad(pos, scaled_size, 0);

        //         mvp = mul_mat4(dc->camera->projection, mvp);

        //     }
        // }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}