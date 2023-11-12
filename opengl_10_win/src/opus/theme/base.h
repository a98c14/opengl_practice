#pragma once
#include <core/defines.h>
#include <core/memory.h>
#include <engine/draw.h>

typedef struct 
{
    BorderRadius rounded_none;
    BorderRadius rounded_sm;
    BorderRadius rounded_md;
    BorderRadius rounded_lg;
    BorderRadius rounded_xl;

    StyleRect rect_default;

    StyleText font_default;
    StyleText font_default_light;
    StyleText font_debug;
    StyleText font_window_header;

} Theme;

internal Theme*
theme_init_default(Arena* arena, Renderer* renderer);