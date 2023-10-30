#pragma once
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "opus_base.h"
#include "opus_string.h"
#include "opus_math.h"

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
    uint32 gl_program_id;
    uint32 uniform_buffer_id;
    uint32 uniform_data_size;

    uint32 location_model;
    uint32 location_shader;
} Material;

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

internal Camera
camera_new(float32 width, float32 height, float32 near, float32 far, float32 window_width, float32 window_height);

internal uint32
shader_load(String vertex_shader_text, String fragment_shader_text);

internal Material
material_new(Arena* arena, String vertex_shader_text, String fragment_shader_text, usize uniform_data_size);

internal Texture
texture_load(uint32 width, uint32 height, uint32 channels, uint32 filter, void* data);