#pragma once
#include <glad/gl.h>
#include <core/log.h>
#include <core/defines.h>
#include <core/strings.h>
#include <core/math.h>
#include <core/hash.h>
#include <core/asserts.h>

/* Constants */ 
#define MATERIAL_CAPACITY 32
#define TEXTURE_CAPACITY 32
#define LAYER_CAPACITY 16

#define MATERIAL_DRAW_BUFFER_CAPACITY 512
#define MATERIAL_DRAW_BUFFER_ELEMENT_CAPACITY 16384

enum
{
    ViewTypeWorld,
    ViewTypeScreen,
    ViewTypeCOUNT,
};
typedef int8 ViewType;
typedef int8 FrameBufferIndex;
typedef int8 TextureIndex;
typedef int32 MaterialIndex;
typedef int16 MaterialDrawBufferIndex;
#define TEXTURE_INDEX_NULL 0
#define FRAME_BUFFER_INDEX_DEFAULT 0
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
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;
} Color;

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
    uint32 location_texture;
} Material;

typedef struct
{
    Vec4 clear_color;
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
    Mat4* model_buffer;
    int32 capacity;
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
    float32 timer;
    RendererDrawState* draw_state;

    // resources
    uint32 frame_buffer_count;
    FrameBuffer* frame_buffers;

    uint32 texture_count;
    Texture* textures;

    uint8 material_count;
    Material* materials;
} Renderer;

typedef struct
{
    uint32 window_width;
    uint32 window_height;

    float32 world_height;
    Color clear_color;
} RendererConfiguration;

internal Renderer*
renderer_new(Arena* arena, RendererConfiguration* configuration);

internal RendererDrawState* 
renderer_draw_state_new(Arena* arena);

internal Camera
camera_new(float32 width, float32 height, float32 near, float32 far, float32 window_width, float32 window_height);

internal uint32
shader_load(String vertex_shader_text, String fragment_shader_text);

internal MaterialIndex
material_new(Renderer* renderer, String vertex_shader_text, String fragment_shader_text, usize uniform_data_size);

internal TextureIndex
texture_new(Renderer* renderer, uint32 width, uint32 height, uint32 channels, uint32 filter, void* data);

internal MaterialDrawBuffer* 
renderer_get_material_buffer(Renderer* renderer, ViewType view_type, FrameBufferIndex layer, TextureIndex texture, MaterialIndex material_index);

internal DrawBuffer 
renderer_buffer_request(Renderer* renderer, FrameBufferIndex layer, MaterialIndex material_index, ViewType view_type, TextureIndex texture, uint32 count);

internal void
frame_buffer_begin(FrameBuffer* frame_buffer);

internal FrameBufferIndex
renderer_frame_buffer_init(Renderer* renderer, uint32 width, uint32 height, uint32 filter, Color clear_color);

internal Vec4
color_to_vec4(Color color);

internal void 
renderer_render(Renderer* renderer, float32 dt);

internal void
texture_shader_data_set(Renderer* renderer, const Texture* texture);