#include "base.h"

internal Theme*
theme_init_default(Arena* arena, Renderer* renderer)
{
    Theme* t = arena_push_struct_zero_aligned(arena, Theme, 16);
    t->rounded_none = (BorderRadius){ .bl =  0, .br = 0, .tr = 0, .tl = 0 };
    t->rounded_sm   = (BorderRadius){ .bl =  2, .br = 2, .tr = 2, .tl = 2 };
    t->rounded_md   = (BorderRadius){ .bl = 10, .br = 10, .tr = 10, .tl = 10 };
    t->rounded_lg   = (BorderRadius){ .bl = 20, .br = 20, .tr = 20, .tl = 20 };
    t->rounded_xl   = (BorderRadius){ .bl = 30, .br = 30, .tr = 30, .tl = 30 };
    
    t->rect_default = (StyleRect){ .softness = 2, .color = color_to_vec4(ColorWhite), .border_radius = t->rounded_sm };

    t->font_default = (StyleText){ .color = color_to_vec4(ColorBlack), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = 24.0f * renderer->pixel_per_unit, .base_line = 1.5f};
    t->font_default_light = (StyleText){ .color = color_to_vec4(ColorWhite), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = 20.0f * renderer->pixel_per_unit, .base_line = 1.5f};
    t->font_window_header = (StyleText){ .color = color_to_vec4(ColorSlate200), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = 18.0f * renderer->pixel_per_unit, .base_line = 1.5f};    
    t->font_debug = (StyleText){ .color = color_to_vec4(ColorSlate100), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = 20.0f * renderer->pixel_per_unit, .base_line = 1.5f};
    return t;
}
