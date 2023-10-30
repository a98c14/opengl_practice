#include "opus_base.h"
#include "opus_graphics.h"

internal Camera
camera_new(float32 width, float32 height, float32 near, float32 far, float32 window_width, float32 window_height)
{
    Camera result;
    result.projection = mat4_ortho(width, height, near, far);
    result.view = mat4_identity();
    result.world_width = width;
    result.world_height = height;
    result.window_width = window_width;
    result.window_height = window_height;
    return result;
}

internal uint32
shader_load(String vertex_shader_text, String fragment_shader_text)
{
    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char* const*)(&vertex_shader_text.value), NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char* const*)(&fragment_shader_text.value), NULL);
    glCompileShader(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    return program;
}

internal Material
material_new(Arena* arena, String vertex_shader_text, String fragment_shader_text)
{
    uint32 program = shader_load(vertex_shader_text, fragment_shader_text);
    int32 mvp_location = glGetUniformLocation(program, "mvp");
    int32 color_location = glGetUniformLocation(program, "u_color");
    Material result;
    result.program_id = program;
    result.location_model = mvp_location;
    result.location_color = color_location;
    return result;
}

internal Texture
texture_load(uint32 width, uint32 height, uint32 channels, uint32 filter, void* data)
{
    Texture texture = {0};
    glGenTextures(1, &texture.gl_texture_id);
    glBindTexture(GL_TEXTURE_2D, texture.gl_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    texture.gl_texture_type = GL_TEXTURE_2D;
    texture.width = width;
    texture.height = height;
    texture.channels = channels;
    texture.layer_count = 1;

    switch (channels)
    {
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
            break;
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
            break;
        default:
            break;
    }

    return texture;
}