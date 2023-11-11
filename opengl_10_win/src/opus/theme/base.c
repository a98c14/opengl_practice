#include "base.h"

internal Theme*
theme_init_default(Arena* arena, Renderer* renderer)
{
    Theme* t = arena_push_struct_zero_aligned(arena, Theme, 16);
    t->rounded_sm   = (BorderRadius){ .bl = 0.05, .br = 0.05, .tr = 0.05, .tl = 0.05 };
    t->rounded_md   = (BorderRadius){ .bl = 0.1, .br = 0.1, .tr = 0.1, .tl = 0.1 };
    t->rounded_lg   = (BorderRadius){ .bl = 0.2, .br = 0.2, .tr = 0.2, .tl = 0.2 };
    t->rounded_xl   = (BorderRadius){ .bl = 0.3, .br = 0.3, .tr = 0.3, .tl = 0.3 };
    
    t->rect_default = (StyleRect){ .softness = 0.02, .color = color_to_vec4(ColorWhite), .border_radius = t->rounded_sm };

    t->font_default = (StyleText){ .color = color_to_vec4(ColorBlack), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = 20.0f * renderer->pixel_per_unit};
    t->font_debug = (StyleText){ .color = color_to_vec4(ColorSlate100), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = 20.0f * renderer->pixel_per_unit};
    return t;
}
