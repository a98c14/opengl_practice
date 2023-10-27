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
    uint32 index_count;
    uint32 vertex_array_object;
} Geometry;

internal uint32
material_new(String vertex_shader_text, String fragment_shader_text);
