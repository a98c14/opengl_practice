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
    // TODO: test these
    // glDeleteShader(vertex_shader);
    // glDeleteShader(fragment_shader);

    return program;
}

internal Material
material_new(Arena* arena, String vertex_shader_text, String fragment_shader_text, usize uniform_data_size)
{
    Material result;
    result.gl_program_id = shader_load(vertex_shader_text, fragment_shader_text);
    result.location_model = glGetUniformLocation(result.gl_program_id, "u_mvp");
    result.uniform_data_size = uniform_data_size;

    // generate custom shader data UBO
    glGenBuffers(1, &result.uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, result.uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, uniform_data_size, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    unsigned int global_ubo_index = glGetUniformBlockIndex(result.gl_program_id, "Global");
    glUniformBlockBinding(result.gl_program_id, global_ubo_index, BINDING_SLOT_GLOBAL);

    unsigned int texture_ubo_index = glGetUniformBlockIndex(result.gl_program_id, "Texture");
    glUniformBlockBinding(result.gl_program_id, texture_ubo_index, BINDING_SLOT_TEXTURE);
    
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