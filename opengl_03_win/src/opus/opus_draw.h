#pragma once

#include "opus_base.h"
#include "opus_math.h"
#include "opus_graphics.h"
#include "opus_graphics_primitives.h"
#include "opus_file.h"

typedef struct
{
    Camera* camera;

    /* Materials */
    Material material_basic;
    Material material_line;
    Material material_quad;
    Material material_circle;
    Material material_boid;
} DrawContext;

internal DrawContext*
draw_context_new(Arena* arena, Camera* camera);

internal void
draw_line(DrawContext* dc, Vec2 start, Vec2 end);