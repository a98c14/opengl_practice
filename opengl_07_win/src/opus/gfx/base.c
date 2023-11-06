#include "base.h"

internal Renderer*
renderer_new(Arena* arena, RendererConfiguration* configuration)
{
    Renderer* renderer      = arena_push_struct_zero_aligned(arena, Renderer, 16);
    renderer->arena         = arena;
    renderer->window_width  = configuration->window_width;
    renderer->window_height = configuration->window_height;
    renderer->frame_buffers = arena_push_array_zero_aligned(arena, FrameBuffer, LAYER_CAPACITY, 16);
    renderer->materials = arena_push_array_zero(arena, Material, MATERIAL_CAPACITY);
    renderer->textures = arena_push_array_zero(arena, Texture, TEXTURE_CAPACITY);
    
    glViewport(0, 0, renderer->window_width, renderer->window_height);
    float32 aspect = renderer->window_width / (float)renderer->window_height;
    float32 world_height = configuration->world_height;
    float32 world_width = world_height * aspect;

    renderer->camera = camera_new(world_width, world_height, 1, -1, renderer->window_width, renderer->window_height);

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    Vec4 clear_color = color_to_vec4(configuration->clear_color);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);

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

    /* Draw State Setup */
    renderer->draw_state = renderer_draw_state_new(arena);

    // Reserve the first slot for NULL texture
    renderer->texture_count += 1;

    // Initialize default frame buffer
    renderer->frame_buffer_count += 1;
    FrameBuffer* frame_buffer = &renderer->frame_buffers[FRAME_BUFFER_INDEX_DEFAULT];
    frame_buffer->texture_index = TEXTURE_INDEX_NULL;
    frame_buffer->buffer_id = 0;
    frame_buffer->clear_color = clear_color;
    frame_buffer->width = configuration->window_width;
    frame_buffer->height = configuration->window_height;

    return renderer;
}

internal RendererDrawState*
renderer_draw_state_new(Arena* arena)
{
    RendererDrawState* draw_state = arena_push_struct_zero(arena, RendererDrawState);
    draw_state->material_draw_buffers = arena_push_array_zero_aligned(arena, MaterialDrawBuffer, MATERIAL_DRAW_BUFFER_CAPACITY, 16);
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

internal MaterialIndex
material_new(Renderer* renderer, String vertex_shader_text, String fragment_shader_text, usize uniform_data_size)
{
    MaterialIndex material_index = renderer->material_count;
    renderer->material_count++;
    Material* result = &renderer->materials[material_index];
    result->gl_program_id = shader_load(vertex_shader_text, fragment_shader_text);
    result->location_model = glGetUniformLocation(result->gl_program_id, "u_mvp");
    result->location_texture = glGetUniformLocation(result->gl_program_id, "u_main_texture");
    result->uniform_data_size = uniform_data_size;
    result->is_initialized = 1;

    // generate custom shader data UBO
    glGenBuffers(1, &result->uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, result->uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, uniform_data_size, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    unsigned int global_ubo_index = glGetUniformBlockIndex(result->gl_program_id, "Global");
    glUniformBlockBinding(result->gl_program_id, global_ubo_index, BINDING_SLOT_GLOBAL);

    unsigned int texture_ubo_index = glGetUniformBlockIndex(result->gl_program_id, "Texture");
    glUniformBlockBinding(result->gl_program_id, texture_ubo_index, BINDING_SLOT_TEXTURE);

    return material_index;
}

internal TextureIndex
texture_new(Renderer* renderer, uint32 width, uint32 height, uint32 channels, uint32 filter, void* data)
{
    TextureIndex texture_index = renderer->texture_count;
    renderer->texture_count++;
    Texture* texture = &renderer->textures[texture_index];
    glGenTextures(1, &texture->gl_texture_id);
    glBindTexture(GL_TEXTURE_2D, texture->gl_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    texture->gl_texture_type = GL_TEXTURE_2D;
    texture->width = width;
    texture->height = height;
    texture->channels = channels;
    texture->layer_count = 1;

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

    return texture_index;
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
            xassert(material->is_initialized, "[ERROR]: Material isn't initialized");
            buffer->key = key;
            buffer->element_count = 0;
            buffer->material_index = material_index;
            buffer->model_buffer = arena_push_array_zero_aligned(renderer->arena, Mat4, MATERIAL_DRAW_BUFFER_ELEMENT_CAPACITY, 16);
            buffer->shader_data_buffer = arena_push_zero_aligned(renderer->arena, MATERIAL_DRAW_BUFFER_ELEMENT_CAPACITY * material->uniform_data_size, 16);

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
                xassert(renderer->draw_state->layer_count < LAYER_CAPACITY, "[ERROR] Draw layer buffer index exceeded capacity!");
            }

            xassert(layer_buffer_index != -1, "[ERROR] LayerDrawBuffer could not be found");
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
                xassert(layer_draw_buffer->view_count <= ViewTypeCOUNT, "[ERROR] Draw view buffer index exceeded capacity!");
            }

            ViewDrawBuffer* view_draw_buffer = &layer_draw_buffer->view_buffers[view_buffer_index];
            xassert(view_buffer_index != -1, "[ERROR]: ViewDrawBuffer could not be found");

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
                xassert(view_draw_buffer->texture_count < TEXTURE_CAPACITY, "[ERROR] Draw texture buffer index exceeded capacity!");
            }

            TextureDrawBuffer* texture_draw_buffer = &view_draw_buffer->texture_draw_buffers[texture_buffer_index];

            // create material buffer
            int32 material_internal_index = texture_draw_buffer->material_count;
            xassert(material_index < MATERIAL_CAPACITY, "[ERROR]: Material internal index exceeded MATERIAL_CAPACITY");

            texture_draw_buffer->material_buffer_indices[material_internal_index];
            texture_draw_buffer->material_buffer_indices[material_internal_index] = i;
            texture_draw_buffer->material_count++;
            break;
        }
    }

    xassert(buffer->key == key, "[ERROR] MaterialDrawBuffer could not be found");
    return buffer;
}

/* queues N elements from underlying draw buffer and returns the addresses */
internal DrawBuffer
renderer_buffer_request(Renderer* renderer, FrameBufferIndex layer, MaterialIndex material_index, ViewType view_type, TextureIndex texture, uint32 count)
{
    xassert(count > 0, "[ERROR] Requested render buffer can not have 0 size");

    MaterialDrawBuffer* buffer = renderer_get_material_buffer(renderer, view_type, layer, texture, material_index);
    const Material* material = &renderer->materials[material_index];
    DrawBuffer result = {0};
    result.capacity = min(count, MATERIAL_DRAW_BUFFER_ELEMENT_CAPACITY - buffer->element_count);
    result.model_buffer = buffer->model_buffer + buffer->element_count;
    result.uniform_data_buffer = (uint8*)buffer->shader_data_buffer + (buffer->element_count * material->uniform_data_size);
    result.uniform_data_size = material->uniform_data_size;
    buffer->element_count += count;
    xassert(result.capacity > 0, "[ERROR]: Material draw buffer is at maximum capacity, can not reserve more elements");
    return result;
}

internal void
frame_buffer_begin(FrameBuffer* frame_buffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->buffer_id);
    glViewport(0, 0, frame_buffer->width, frame_buffer->height);
    glClearColor(frame_buffer->clear_color.x, frame_buffer->clear_color.y, frame_buffer->clear_color.z, frame_buffer->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

internal FrameBufferIndex
renderer_frame_buffer_init(Renderer* renderer, uint32 width, uint32 height, uint32 filter, Color clear_color)
{
    TextureIndex texture_index = texture_new(renderer, width, height, 4, filter, NULL);

    uint32 fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->textures[texture_index].gl_texture_id, 0);

    // create render buffer
    uint32 rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("[ERROR]: frame_buffer is not complete");
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // init frame buffer
    FrameBufferIndex layer_index = renderer->frame_buffer_count;
    FrameBuffer* frame_buffer = &renderer->frame_buffers[layer_index];
    frame_buffer->buffer_id = fbo;
    frame_buffer->texture_index = texture_index;
    frame_buffer->width = width;
    frame_buffer->height = height;
    frame_buffer->clear_color = color_to_vec4(clear_color);
    renderer->frame_buffer_count++;

    return layer_index;
}

internal Vec4
color_to_vec4(Color c)
{
    return (Vec4){ .r = c.r / 255.0F, .g = c.g / 255.0F, .b = c.b / 255.0F, .a = c.a / 255.0F };
}

internal void 
renderer_render(Renderer* renderer, float32 dt)
{
    Camera* camera = &renderer->camera;
    RendererDrawState* state = renderer->draw_state;
    renderer->timer += dt;

    /* setup global shader data */
    GlobalUniformData global_shader_data = {0};
    global_shader_data.time = renderer->timer;
    glBindBuffer(GL_UNIFORM_BUFFER, renderer->global_uniform_buffer_id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GlobalUniformData), &global_shader_data);

    // TODO(selim): Actually geometries should be in draw buffers as well but since I only use 
    // quads its not necessary for now
    // glBindVertexArray(renderer->quad_vao);

    /* Layer */
    for(int layer_index = 0; layer_index < state->layer_count; layer_index++)
    {
        LayerDrawBuffer* layer_draw_buffer = &state->layer_draw_buffers[layer_index];
        FrameBuffer* frame_buffer = &renderer->frame_buffers[layer_draw_buffer->layer_index];
        frame_buffer_begin(frame_buffer);

        /* View */
        for(int view_type_index = 0; view_type_index < ViewTypeCOUNT; view_type_index++)
        {
            ViewDrawBuffer* view_draw_buffer = &layer_draw_buffer->view_buffers[view_type_index];
            // Mat4 view_matrix = view_draw_buffer->view_type == ViewTypeWorld ? camera->view : mat4_identity();

            /* Texture */
            for(int texture_index = 0; texture_index < view_draw_buffer->texture_count; texture_index++)
            {
                TextureDrawBuffer* texture_draw_buffer = &view_draw_buffer->texture_draw_buffers[texture_index];
                if(texture_draw_buffer->texture_index != TEXTURE_INDEX_NULL)
                {
                    Texture* texture = &renderer->textures[texture_draw_buffer->texture_index];
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(texture->gl_texture_type, texture->gl_texture_id);
                    texture_shader_data_set(renderer, texture);
                }

                // material
                for(int material_index = 0; material_index < texture_draw_buffer->material_count; material_index++)
                {
                    MaterialDrawBufferIndex material_draw_buffer_index = texture_draw_buffer->material_buffer_indices[material_index];
                    MaterialDrawBuffer* material_draw_buffer = &state->material_draw_buffers[material_draw_buffer_index];
                    Material* material = &renderer->materials[material_draw_buffer->material_index];

                    glUseProgram(material->gl_program_id);
                    glUniform1i(material->location_texture, 0);

                    // models
                    glBindBuffer(GL_UNIFORM_BUFFER, material->uniform_buffer_id);
                    glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_SLOT_CUSTOM, material->uniform_buffer_id, 0, material->uniform_data_size);

                    for(int element_index = 0; element_index < material_draw_buffer->element_count; element_index++)
                    {
                        void* shader_data = ((uint8*)material_draw_buffer->shader_data_buffer + element_index * material->uniform_data_size);
                        glBufferSubData(GL_UNIFORM_BUFFER, 0, material->uniform_data_size, shader_data);

                        Mat4 model = material_draw_buffer->model_buffer[element_index];
                        Mat4 mvp = mul_mat4(camera->view, model);
                        mvp = mul_mat4(camera->projection, mvp);
                        glUniformMatrix4fv(material->location_model, 1, GL_FALSE, mvp.v);
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    }
                    
                    material_draw_buffer->element_count = 0;
                }
            }
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

internal void
texture_shader_data_set(Renderer* renderer, const Texture* texture)
{
    TextureUniformData shader_data = {0};
    shader_data.layer_count = texture->layer_count;
    shader_data.size = (Vec2){ .x = (float32)texture->width, .y = (float32)texture->height };
    glBindBuffer(GL_UNIFORM_BUFFER, renderer->texture_uniform_buffer_id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(TextureUniformData), &shader_data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}