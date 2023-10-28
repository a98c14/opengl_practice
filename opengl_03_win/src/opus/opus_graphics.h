#pragma once
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "opus_base.h"
#include "opus_string.h"
#include "opus_math.h"

typedef struct
{
    Mat3 projection;
    Mat3 view;
} Camera2D;

typedef struct
{
    Mat4 projection;
    Mat4 view;
} Camera;

typedef struct
{
    uint32 index_count;
    uint32 vertex_array_object;
} Geometry;

typedef struct
{
    uint32 program_id;
    uint32 location_model;
    uint32 location_color;
} Material;

internal Camera
camera_new(float32 width, float32 height, float32 near, float32 far);

internal uint32
shader_load(String vertex_shader_text, String fragment_shader_text);

internal Material
material_new(Arena* arena, String vertex_shader_text, String fragment_shader_text);
