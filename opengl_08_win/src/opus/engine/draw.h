#pragma once

#include <core/defines.h>
#include <core/math.h>
#include <core/file.h>
#include <gfx/base.h>
#include <gfx/primitives.h>
#include <gfx/math.h>
#include <gfx/utils.h>
#include <fonts.h>
#include "color.h"
#include "text.h"

typedef struct
{
    Renderer* renderer;
    Camera* camera;

    /* Geometries */
    Geometry geometry_quad;

    /* Materials */
    MaterialIndex material_text;
    MaterialIndex material_basic;
    MaterialIndex material_line;
    MaterialIndex material_quad;
    MaterialIndex material_circle;
    MaterialIndex material_boid;

    /* Font */
    GlyphAtlas* font_open_sans;
} DrawContext;

typedef struct 
{
    Vec4 glyph_bounds;
    Vec4 color;
    Vec4 outline_color;
    /** Softness of the glyph edges. Recommended value: 30 */
    float32 softness;
    /** Thickness of the glyph. Recommended value: 0.5, Values outside of the range 0.4 and 0.9 are not really usable */
    float32 thickness;
    /** Outline thickness. Should be between 0 and 0.5 */
    float32 outline_thickness;
    float32 _;
} ShaderDataText;

typedef struct 
{
    Vec4 color;
} ShaderDataLine;

typedef struct 
{
    Vec4 color;
    float32 fill_ratio;
} ShaderDataCircle;

typedef struct 
{
    Vec4 color;
} ShaderDataBoid;

internal DrawContext*
draw_context_new(Arena* arena, Renderer* renderer);

internal void
draw_line(DrawContext* dc, Vec2 start, Vec2 end, Color color);

internal void
draw_bounds(DrawContext* dc, float32 left, float32 right, float32 bottom, float32 top, Color color);

internal void
draw_text(DrawContext* dc, Vec2 pos, String str, Color color);

internal void
draw_circle(DrawContext* dc, Vec2 position, float32 radius, Color color);

internal void
draw_boid(DrawContext* dc, Vec2 position, Vec2 direction, float32 size, Color color);