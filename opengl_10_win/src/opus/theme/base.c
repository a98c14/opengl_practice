#include "base.h"

internal Theme*
theme_init_default(Arena* arena, Renderer* renderer)
{
    Theme* t = arena_push_struct_zero_aligned(arena, Theme, 16);
    t->alignment = AlignmentTopLeft;
    t->padding = vec2(4, 4);
    t->line_height = 8;
    t->spacing = 2;

    t->rounded_none = (BorderRadius){ .bl =  0, .br = 0, .tr = 0, .tl = 0 };
    t->rounded_sm   = (BorderRadius){ .bl =  2, .br = 2, .tr = 2, .tl = 2 };
    t->rounded_md   = (BorderRadius){ .bl = 10, .br = 10, .tr = 10, .tl = 10 };
    t->rounded_lg   = (BorderRadius){ .bl = 20, .br = 20, .tr = 20, .tl = 20 };
    t->rounded_xl   = (BorderRadius){ .bl = 30, .br = 30, .tr = 30, .tl = 30 };
    
    t->rect_default = (StyleRect){ .softness = 2, .color = color_to_vec4(ColorWhite), .border_radius = t->rounded_sm };
    t->rect_debug = (StyleRect){ .border_thickness = 2, .softness = 0, .color = color_to_vec4(ColorInvisible), .border_color = color_to_vec4(ColorRed600), .border_radius = t->rounded_none };
    t->rect_view = (StyleRect){ .softness = 2, .color = color_to_vec4(ColorWhite100), .border_radius = { .bl = 3, .br = 3, .tr = 0, .tl = 0 } };
    t->rect_header = (StyleRect){ .border_thickness = 0, .softness = 2, .color = color_to_vec4(ColorSlate700), .border_color = color_to_vec4(ColorInvisible), .border_radius = { .bl = 0, .br = 0, .tr = 3, .tl = 3 } };
    t->rect_slider_bar = (StyleRect){ .softness = 2, .color = color_to_vec4(ColorBlack), .border_radius = t->rounded_sm };
    t->rect_button = (StyleRect){ .softness = 2, .color = color_to_vec4(ColorAmber600), .border_radius = t->rounded_sm };
    t->rect_button_hover = (StyleRect){ .softness = 2, .color = color_to_vec4(ColorAmber400), .border_radius = t->rounded_sm };

    t->rect_header_hover = t->rect_header;
    t->rect_header_hover.color = color_to_vec4(ColorSlate500);

    t->font_default = (StyleText){ .color = color_to_vec4(ColorBlack), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = em(18), .base_line = em(3)};
    t->font_default_light = (StyleText){ .color = color_to_vec4(ColorWhite), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = em(16), .base_line = em(3)};
    t->font_window_header = (StyleText){ .color = color_to_vec4(ColorSlate200), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = em(16), .base_line = em(3)};    
    t->font_debug = (StyleText){ .color = color_to_vec4(ColorSlate100), .outline_color = color_to_vec4(ColorInvisible), .softness = 30, .thickness = 0.50, .outline_thickness = 0.20, .font_size = em(16), .base_line = em(3)};

    t->label_default = (StyleLabel){.font = t->font_default, .padding = vec2(2, 2), .alignment = AlignmentCenter, .background = t->rect_header };
    return t;
}
