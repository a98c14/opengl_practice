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
    Material material_text;
    Material material_text_v2;
    Material material_basic;
    Material material_line;
    Material material_quad;
    Material material_circle;
    Material material_boid;
} DrawContext;

typedef struct 
{
    Vec4 glyph_bounds;
    Vec4 color;
    Vec4 outline_color;
    /** Softness of the glyph edges. Recommended value: 30 */
    float32 softness;
    /** Thickness of the glyph. Recommended value: 0.5, Values outside of the range 0.4 and 0.9 are not really usable */
    float u_thickness;
    /** Outline thickness. Should be between 0 and 0.5 */
    float32 outline_thickness;
} ShaderDataText;

internal DrawContext*
draw_context_new(Arena* arena, Camera* camera);

internal void
draw_line(DrawContext* dc, Vec2 start, Vec2 end);

internal void
draw_bounds(DrawContext* dc, float32 left, float32 right, float32 bottom, float32 top);