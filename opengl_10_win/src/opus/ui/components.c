#include "components.h"

internal bool32
ui_slider(UIContext* ctx, String label, Range range, float32* value)
{
    // TODO: use an actual id
    int32 name_hash = hash_string(label);
    UIID id = uuid_new(name_hash, 0);
    ui_text(ctx, string_pushf(ctx->frame_arena, "%s: %0.2f", label.value, *value));

    UIFrame* frame = ui_active_frame(ctx);
    Rect slider_row = ui_row(ctx, frame);
    slider_row = rect_shrink(slider_row, vec2(ctx->theme->padding.x*2, 5));
    draw_rect(ctx->dc, slider_row, 0, 1, ctx->theme->rect_slider_bar);

    float32 normalized = (*value - range.min) / (range.max - range.min);
    float32 left = rect_left(slider_row);
    float32 right = rect_right(slider_row);
    float32 x = left + slider_row.w * normalized;
    Circle handle = circle(vec2(x, slider_row.y), em(20));
    bool32 hover = intersects_circle_point(handle, ctx->mouse.world);
    if(hover && ui_is_free(ctx) && input_mouse_pressed(ctx->mouse, MouseButtonStateLeft))
    {
        ui_activate(ctx, id);
        ctx->drag_offset = sub_vec2(ctx->mouse.world, handle.center);
    }
    else if(ui_is_active(ctx, id) && input_mouse_pressed(ctx->mouse, MouseButtonStateLeft))
    {
        float32 new_x = sub_vec2(ctx->mouse.world, ctx->drag_offset).x;
        new_x = clamp(left, new_x, right);
        *value = (new_x - left) / slider_row.w * (range.max - range.min);
    }
    else if(ui_is_active(ctx, id) && input_mouse_released(ctx->mouse, MouseButtonStateLeft))
    {
        ui_active_clear(ctx);
    }

    Color c = hover ? ColorRed400 : ColorRed600;
    draw_circle_filled(ctx->dc, handle, c);

    return false;
}
