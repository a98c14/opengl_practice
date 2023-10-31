#pragma once
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "opus_base.h"
#include "opus_string.h"
#include "opus_math.h"
#include "opus_log.h"

/* Constants */ 
#define MATERIAL_CAPACITY 32
#define TEXTURE_CAPACITY 32
#define LAYER_CAPACITY 16

#define MATERIAL_DRAW_BUFFER_CAPACITY 512
#define MATERIAL_DRAW_BUFFER_ELEMENT_CAPACITY 8192

enum
{
    ViewTypeWorld,
    ViewTypeScreen,
    ViewTypeCOUNT,
};
typedef int8 ViewType;
typedef int8 FrameBufferIndex;
typedef int8 TextureIndex;
typedef int8 MaterialIndex;
typedef int16 MaterialDrawBufferIndex;
#define TEXTURE_INDEX_NULL 0
#define TEXTURE_NULL 0
#define MATERIAL_DRAW_BUFFER_EMPTY_KEY -1
#define MATERIAL_DRAW_BUFFER_MAX_PROBE 5

#define BINDING_SLOT_GLOBAL 0
#define BINDING_SLOT_TEXTURE 1
#define BINDING_SLOT_CUSTOM 2

typedef struct
{
    Mat3 projection;
    Mat3 view;
} Camera2D;

typedef struct
{
    Mat4 projection;
    Mat4 view;

    float32 window_width;
    float32 window_height;

    float32 world_height;
    float32 world_width;
} Camera;

typedef struct
{
    uint32 index_count;
    uint32 vertex_array_object;
} Geometry;

typedef struct
{
    bool32 is_initialized;

    uint32 gl_program_id;
    uint32 uniform_buffer_id;
    uint32 uniform_data_size;

    uint32 location_model;
    uint32 location_shader;
} Material;

typedef struct
{
    Vec4   clear_color;
    uint32 width;
    uint32 height;
    uint32 buffer_id;
    TextureIndex texture_index;
} FrameBuffer;

typedef struct
{
    int32 width;
    int32 height;
    int32 channels;
    int32 layer_count;
    uint32 gl_texture_id;
    uint32 gl_texture_type;
} Texture;

typedef struct
{
    float32 time;
    Vec3 _;
} GlobalUniformData;

typedef struct
{
    Vec2 size;
    float32 layer_count;
    float32 _;
} TextureUniformData;

enum ShaderProgramType
{
    ShaderProgramTypeVertex   = GL_VERTEX_SHADER,
    ShaderProgramTypeFragment = GL_FRAGMENT_SHADER
};

typedef struct
{
    uint64 key;
    MaterialIndex material_index;

    uint32 element_count;
    Mat4* model_buffer;
    void* shader_data_buffer;
} MaterialDrawBuffer;

typedef struct
{
    TextureIndex texture_index;

    uint32 material_count;
    MaterialDrawBufferIndex material_buffer_indices[MATERIAL_CAPACITY];
} TextureDrawBuffer;

typedef struct
{
    ViewType view_type;

    uint32 texture_count;
    TextureDrawBuffer texture_draw_buffers[TEXTURE_CAPACITY];
} ViewDrawBuffer;

typedef struct
{
    FrameBufferIndex layer_index;

    // grouped by view type (screen, world)
    uint32 view_count;
    ViewDrawBuffer view_buffers[ViewTypeCOUNT];
} LayerDrawBuffer;

typedef struct
{
    // model and shader data that is needed to render the image
    // indexed by hash of layer, view, texture, material
    uint32 material_draw_buffer_count;
    MaterialDrawBuffer* material_draw_buffers;

    // draw buffers
    uint32 layer_count;
    LayerDrawBuffer layer_draw_buffers[LAYER_CAPACITY];
} RendererDrawState;

typedef struct
{
    int32 capacity;
    Mat4* model_buffer;

    uint32 uniform_data_size;
    void* uniform_data_buffer;
} DrawBuffer;

typedef struct
{
    Arena* arena;

    float32 window_width;
    float32 window_height;

    uint32 global_uniform_buffer_id;
    uint32 texture_uniform_buffer_id;

    Camera camera;
    RendererDrawState* draw_state;

    // resources
    uint32 frame_buffer_count;
    FrameBuffer frame_buffers[LAYER_CAPACITY];

    uint32 texture_count;
    Texture textures[TEXTURE_CAPACITY];

    uint8 material_count;
    Material materials[MATERIAL_CAPACITY];
} Renderer;

typedef struct
{
    uint32 window_width;
    uint32 window_height;

    float32 world_height;
    Vec4 clear_color;
} RendererConfiguration;

internal Renderer*
renderer_new(Arena* arena, RendererConfiguration* configuration);

internal RendererDrawState* 
renderer_draw_state_new(Arena* arena);

internal Camera
camera_new(float32 width, float32 height, float32 near, float32 far, float32 window_width, float32 window_height);

internal uint32
shader_load(String vertex_shader_text, String fragment_shader_text);

internal Material
material_new(Arena* arena, String vertex_shader_text, String fragment_shader_text, usize uniform_data_size);

internal Texture
texture_load(uint32 width, uint32 height, uint32 channels, uint32 filter, void* data);

internal MaterialDrawBuffer* 
renderer_get_material_buffer(Renderer* renderer, ViewType view_type, FrameBufferIndex layer, TextureIndex texture, MaterialIndex material_index);

internal DrawBuffer 
renderer_buffer_request(Renderer* renderer, FrameBufferIndex layer, MaterialIndex material_index, ViewType view_type, TextureIndex texture, uint32 count);
