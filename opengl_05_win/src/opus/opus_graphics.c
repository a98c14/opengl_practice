#include "opus_base.h"
#include "opus_graphics.h"
#include "opus_hash.h"

internal Renderer*
renderer_new(Arena* arena, RendererConfiguration* configuration)
{
    Renderer* renderer      = arena_push_struct_zero(arena, Renderer);
    renderer->arena         = arena;
    renderer->window_width  = configuration->window_width;
    renderer->window_height = configuration->window_height;
    renderer->draw_state    = renderer_draw_state_new(arena);

    glViewport(0, 0, renderer->window_width, renderer->window_height);

    float32 aspect = renderer->window_width / (float)renderer->window_height;
    float32 world_height = configuration->world_height;
    float32 world_width = world_height * aspect;
    renderer->camera = camera_new(world_width, world_height, 1, -1, renderer->window_width, renderer->window_height);

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glClearColor(configuration->clear_color.r, configuration->clear_color.g, configuration->clear_color.b, configuration->clear_color.a);

    /* Create Global UBO */
    glGenBuffers(1, &renderer->global_uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, renderer->global_uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GlobalUniformData), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_SLOT_GLOBAL, renderer->global_uniform_buffer_id, 0, sizeof(GlobalUniformData));

    /* Create Texture UBO */
    glGenBuffers(1, &renderer->texture_uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, renderer->texture_uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(TextureUniformData), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_SLOT_TEXTURE, renderer->texture_uniform_buffer_id, 0, sizeof(TextureUniformData));

    return renderer;
}

internal RendererDrawState*
renderer_draw_state_new(Arena* arena)
{
    RendererDrawState* draw_state = arena_push_struct_zero(arena, RendererDrawState);
    draw_state->material_draw_buffers = arena_push_array_zero(arena, MaterialDrawBuffer, MATERIAL_DRAW_BUFFER_CAPACITY);
    for(int i = 0; i < MATERIAL_DRAW_BUFFER_CAPACITY; i++)
    {
        draw_state->material_draw_buffers[i].key = MATERIAL_DRAW_BUFFER_EMPTY_KEY;
    }

    return draw_state;
}

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
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

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

internal MaterialDrawBuffer*
renderer_get_material_buffer(Renderer* renderer, ViewType view_type, FrameBufferIndex layer, TextureIndex texture, MaterialIndex material_index)
{
    uint64 key = ((uint64)layer << 24) + ((uint64)view_type << 16) + ((uint64)texture << 8) + ((uint64)material_index << 0);
    MaterialDrawBufferIndex index = hash_uint64(key) % MATERIAL_DRAW_BUFFER_CAPACITY;

    MaterialDrawBuffer* buffer = NULL;
    for(int i = index; i < index + MATERIAL_DRAW_BUFFER_MAX_PROBE; i++)
    {
        buffer = &renderer->draw_state->material_draw_buffers[i % MATERIAL_DRAW_BUFFER_CAPACITY];

        // if target buffer is found, exit
        if(buffer->key == key)
            break;

        // if the slot is empty and we are still in probe range, initialize the buffer
        if(buffer->key == MATERIAL_DRAW_BUFFER_EMPTY_KEY)
        {
            log_info("Initializing material buffer. Material: %d, Layer: %d, Texture: %d, View: %d", material_index, layer, texture, view_type);
            const Material* material = &renderer->materials[material_index];
            Assert(material->is_initialized, "[ERROR]: Material isn't initialized");
            buffer->key = key;
            buffer->element_count = 0;
            buffer->material_index = material_index;
            buffer->model_buffer = arena_push_array_zero(renderer->arena, Mat4, MATERIAL_DRAW_BUFFER_ELEMENT_CAPACITY);
            buffer->shader_data_buffer = arena_push_zero(renderer->arena, MATERIAL_DRAW_BUFFER_ELEMENT_CAPACITY * material->uniform_data_size);

            // find layer buffer
            int32 layer_buffer_index = -1;
            for(int j = 0; j < renderer->draw_state->layer_count; j++)
            {
                FrameBufferIndex layer_index = renderer->draw_state->layer_draw_buffers[j].layer_index;
                if(layer_index == layer)
                {
                    layer_buffer_index = j;
                    break;
                }
            }

            if(layer_buffer_index == -1)
            {
                layer_buffer_index = renderer->draw_state->layer_count;
                renderer->draw_state->layer_draw_buffers[layer_buffer_index].layer_index = layer;
                renderer->draw_state->layer_count++;
                Assert(renderer->draw_state->layer_count < LAYER_CAPACITY, "[ERROR] Draw layer buffer index exceeded capacity!");
            }

            Assert(layer_buffer_index != -1, "[ERROR] LayerDrawBuffer could not be found");
            LayerDrawBuffer* layer_draw_buffer = &renderer->draw_state->layer_draw_buffers[layer_buffer_index];

            // find view buffer
            int32 view_buffer_index = -1;
            for(int j = 0; j < layer_draw_buffer->view_count; j++)
            {
                ViewType current_view_type = layer_draw_buffer->view_buffers[j].view_type;
                if(current_view_type == view_type)
                {
                    view_buffer_index = j;
                    break;
                }
            }

            if(view_buffer_index == -1)
            {
                view_buffer_index = layer_draw_buffer->view_count;
                layer_draw_buffer->view_buffers[view_buffer_index].view_type = view_type;
                layer_draw_buffer->view_count++;
                Assert(layer_draw_buffer->view_count <= ViewTypeCOUNT, "[ERROR] Draw view buffer index exceeded capacity!");
            }

            ViewDrawBuffer* view_draw_buffer = &layer_draw_buffer->view_buffers[view_buffer_index];
            Assert(view_buffer_index != -1, "[ERROR]: ViewDrawBuffer could not be found");

            // find texture buffer
            int32 texture_buffer_index = -1;
            for(int j = 0; j < view_draw_buffer->texture_count; j++)
            {
                TextureIndex current_texture_index = view_draw_buffer->texture_draw_buffers[j].texture_index;
                if(current_texture_index == texture)
                {
                    texture_buffer_index = j;
                    break;
                }
            }

            if(texture_buffer_index == -1)
            {
                texture_buffer_index = view_draw_buffer->texture_count;
                view_draw_buffer->texture_draw_buffers[texture_buffer_index].texture_index = texture;
                view_draw_buffer->texture_count++;
                Assert(view_draw_buffer->texture_count < TEXTURE_CAPACITY, "[ERROR] Draw texture buffer index exceeded capacity!");
            }

            TextureDrawBuffer* texture_draw_buffer = &view_draw_buffer->texture_draw_buffers[texture_buffer_index];

            // create material buffer
            int32 material_internal_index = texture_draw_buffer->material_count;
            Assert(material_index < MATERIAL_CAPACITY, "[ERROR]: Material internal index exceeded MATERIAL_CAPACITY");

            texture_draw_buffer->material_buffer_indices[material_internal_index];
            texture_draw_buffer->material_buffer_indices[material_internal_index] = i;
            texture_draw_buffer->material_count++;
            break;
        }
    }

    Assert(buffer->key == key, "[ERROR] MaterialDrawBuffer could not be found");
    return buffer;
}

/* queues N elements from underlying draw buffer and returns the addresses */
internal DrawBuffer
renderer_buffer_request(Renderer* renderer, FrameBufferIndex layer, MaterialIndex material_index, ViewType view_type, TextureIndex texture, uint32 count)
{
    Assert(count > 0, "[ERROR] Requested render buffer can not have 0 size");

    MaterialDrawBuffer* buffer = renderer_get_material_buffer(renderer, view_type, layer, texture, material_index);
    const Material* material = &renderer->materials[material_index];
    DrawBuffer result = {0};
    result.capacity = min(count, MATERIAL_DRAW_BUFFER_ELEMENT_CAPACITY - buffer->element_count);
    result.model_buffer = buffer->model_buffer + buffer->element_count;
    result.uniform_data_buffer = (uint8*)buffer->shader_data_buffer + (buffer->element_count * material->uniform_data_size);
    result.uniform_data_size = material->uniform_data_size;
    buffer->element_count += count;
    Assert(result.capacity > 0, "[ERROR]: Material draw buffer is at maximum capacity, can not reserve more elements");
    return result;
}