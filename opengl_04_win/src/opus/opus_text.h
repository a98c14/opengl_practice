#pragma once

#include "opus_base.h"
#include "opus_math.h"
#include "opus_memory.h"
#include "opus_string.h"
#include "opus_graphics.h"

typedef struct
{
    uint32 unicode;
    float32 advance;
    Bounds plane_bounds;
    Bounds atlas_bounds;
} Glyph;

typedef struct
{
    uint16 width;
    uint16 height;
    uint16 size;
    uint16 distance_range;
    float32 line_height;
    float32 ascender;
    float32 descender;
    float32 underline_y;
    float32 underline_thickness;
} GlyphAtlasInfo;

typedef struct
{
    uint32 glyph_count;
    const Glyph* glyphs;
    const GlyphAtlasInfo* atlas_info;
    Texture texture;
} GlyphAtlas;

internal GlyphAtlas*
glyph_atlas_load(Arena* arena, const GlyphAtlasInfo* atlas_info, const Glyph* glyphs, uint32 glyph_count, Texture texture);