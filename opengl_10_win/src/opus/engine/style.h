#pragma once

#include <core/defines.h>
#include <core/math.h>

typedef union 
{
    struct { float32 tr; float32 br; float32 tl; float32 bl; };
    Vec4 v;
} BorderRadius;

typedef struct
{
    Vec4 color;
    BorderRadius border_radius;
    float32 softness;
} StyleRect;

typedef struct
{
    Vec4 color;
    Vec4 outline_color;
    /** Softness of the glyph edges. Recommended value: 30 */
    float32 softness;
    /** Thickness of the glyph. Recommended value: 0.5, Values outside of the range 0.4 and 0.9 are not really usable */
    float32 thickness;
    /** Outline thickness. Should be between 0 and 0.5 */
    float32 outline_thickness;
    float32 font_size;
} StyleText;

internal StyleText
font_style_override_size(StyleText style, float32 new_size);