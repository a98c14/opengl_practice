#include "components.h"

internal Rect
ui_container(UIContext* ctx, Rect container, StyleContainer style)
{
    Rect outer = rect_shrink(container, style.margin);
    Rect inner = rect_shrink(container, style.padding);
    if(style.background.color.a > 0)
        draw_rect(ctx->dc, outer, 0, SORT_LAYER_INDEX_DEFAULT, style.background);
    return inner;
}

internal void
ui_label(UIContext* ctx, Rect container, String str, StyleLabel style)
{
    Rect inner = rect_shrink(container, style.padding);
    if(style.background.color.a > 0)
        draw_rect(ctx->dc, container, 0, SORT_LAYER_INDEX_DEFAULT, style.background);
    draw_text(ctx->dc, rect_relative(inner, style.alignment), str, style.alignment, style.font);
}

internal bool32
ui_slider(UIContext* ctx, String label, Range range, float32* value)
{
    // TODO: use an actual id
    int32 name_hash = hash_string(label);
    UIID id = uuid_new(name_hash, 0);
    UIFrame* frame = ui_active_frame(ctx);
    Rect row = rect(0, 0, frame->cursor.w, ctx->theme->line_height);
    row = rect_anchor(row, frame->cursor, ANCHOR_TL_TL);
    frame->cursor = rect_move(frame->cursor, vec2(0, -row.h));
    Alignment alignment = AlignmentLeft;
    Rect inner_row = rect_shrink(row, vec2(ctx->theme->padding.x*2, 0));
    draw_text(ctx->dc, rect_relative(inner_row, alignment), string_pushf(ctx->frame_arena, "%s: %0.2f", label.value, *value), alignment, ctx->theme->font_default);

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

    return ui_is_active(ctx, id);
}


internal bool32
ui_button(UIContext* ctx, String label)
{
    // TODO: use an actual id
    int32 name_hash = hash_string(label);
    UIID id = uuid_new(name_hash, 0);

    UIFrame* frame = ui_active_frame(ctx);
    Rect row = rect(0, 0, frame->cursor.w, ctx->theme->line_height);
    row = rect_anchor(row, frame->cursor, ANCHOR_TL_TL);

    frame->cursor = rect_move(frame->cursor, vec2(0, -row.h-ctx->theme->spacing));
    Alignment alignment = AlignmentCenter;
    Rect inner_row = rect_shrink(row, vec2(ctx->theme->padding.x*2, 0));
    draw_text(ctx->dc, rect_relative(inner_row, alignment), label, alignment, ctx->theme->font_default);
    bool32 hover = intersects_rect_point(row, ctx->mouse.world);
    bool32 clicked = ui_is_active(ctx, id) && input_mouse_released(ctx->mouse, MouseButtonStateLeft);
    if(hover && ui_is_free(ctx) && input_mouse_pressed(ctx->mouse, MouseButtonStateLeft))
    {
        ui_activate(ctx, id);
    }
    else if(clicked)
    {
        ui_active_clear(ctx);
    }

    StyleRect c = hover ? ctx->theme->rect_button_hover : ctx->theme->rect_button;
    draw_rect(ctx->dc, inner_row, 0, 1, c);
    return clicked;
}


internal void
ui_grid_begin(UIContext* ctx, Rect rect, uint32 rows, uint32 columns)
{
    UIFrame* frame = ui_active_frame(ctx);
    // add bottom padding remove the last element spacing (spacing only needs to be applied between the elements)
    frame->cursor = rect_move(frame->cursor, vec2(0, -ctx->theme->padding.y+ctx->theme->spacing));
    Rect base_rect = rect_wh(frame->base.w, rect_bottom(frame->base)-rect_bottom(frame->cursor));
    base_rect = rect_anchor(base_rect, frame->cursor, ANCHOR_BL_BL);
    draw_rect(ctx->dc, base_rect, 0, 0, ctx->theme->rect_view);
    ctx->frame_count--;
}
