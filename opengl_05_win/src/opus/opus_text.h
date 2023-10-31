#pragma once

#include <string.h>

#include "opus_base.h"
#include "opus_math.h"
#include "opus_memory.h"
#include "opus_string.h"
#include "opus_graphics.h"
#include "opus_graphics_math.h"


typedef enum
{
	RectAlignmentTypeCenter,
	RectAlignmentTypeBottom,
	RectAlignmentTypeTop,
	RectAlignmentTypeRight,
	RectAlignmentTypeLeft,

	RectAlignmentTypeBottomLeft,
	RectAlignmentTypeBottomRight,
	RectAlignmentTypeTopLeft,
	RectAlignmentTypeTopRight,
	RectAlignmentTypeCount
} RectAlignmentType;

                                                                // Center,  Bottom,   Top,  Right,  Left,  BottomLeft,  BottomRight,  TopLeft,  TopRight
const float32 AlignmentMultiplierX[RectAlignmentTypeCount]     = {      0,       0,     0,   -0.5,   0.5,         0.5,         -0.5,      0.5,      -0.5 };
const float32 AlignmentMultiplierY[RectAlignmentTypeCount]     = {      0,     0.5,  -0.5,      0,     0,         0.5,          0.5,     -0.5,      -0.5 };
const float32 FontAlignmentMultiplierX[RectAlignmentTypeCount] = {   -0.5,    -0.5,  -0.5,   -1.0,   0.0,         0.0,         -1.0,      0.0,      -1.0 };
const float32 FontAlignmentMultiplierY[RectAlignmentTypeCount] = {   -0.5,     0.0,  -1.0,   -0.5,  -0.5,         0.0,          0.0,     -1.0,      -1.0 };
const float32 AnchorMultiplierX[RectAlignmentTypeCount]        = {      0,       0,     0,    0.5,  -0.5,        -0.5,          0.5,     -0.5,       0.5 };
const float32 AnchorMultiplierY[RectAlignmentTypeCount]        = {      0,    -0.5,   0.5,      0,     0,        -0.5,         -0.5,      0.5,       0.5 };

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

internal Glyph
glyph_get(GlyphAtlas* atlas, char c);

internal GlyphAtlas*
glyph_atlas_load(Arena* arena, const GlyphAtlasInfo* atlas_info, const Glyph* glyphs, uint32 glyph_count, Texture texture);

internal Rect
text_calculate_bounds(GlyphAtlas* atlas, String str, float32 size_in_pixels);

internal Rect
text_calculate_transforms(GlyphAtlas* atlas, String str, float32 size_in_pixels, Vec2 position, RectAlignmentType alignment, Mat4* dst_matrices, uint32 dst_index);