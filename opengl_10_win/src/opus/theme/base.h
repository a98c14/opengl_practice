#pragma once
#include <core/defines.h>
#include <core/memory.h>
#include <engine/draw.h>

typedef struct 
{
    BorderRadius rounded_sm;
    BorderRadius rounded_md;
    BorderRadius rounded_lg;
    BorderRadius rounded_xl;

    StyleRect style_rect_default;
} Theme;

internal Theme*
theme_init_default(Arena* arena);


