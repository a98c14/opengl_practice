#include "components.h"

internal UIWindow
ui_window(UIContext* ctx, Rect* container, UIID id, String name, bool32* is_expanded, StyleWindow style)
{
    Rect base_container = *container;
    const Vec2 header_padding = vec2(4, 4);
    Rect header = rect_from_wh(base_container.w, em(1) + header_padding.y);
    header = rect_anchor(header, base_container, ANCHOR_TL_TL);
    Rect header_inner = rect_shrink(header, header_padding);

    bool32 hover = intersects_rect_point(header, ctx->mouse.world);
    StyleRect header_style = hover ? style.header_background_hover : style.header_background;
    draw_rect(ctx->dc,header, 0, SORT_LAYER_INDEX_DEFAULT, header_style);
    draw_text(ctx->dc, rect_cl(header_inner), name, AlignmentLeft, style.header_font);
    if(!ui_is_active_any(ctx) && hover && input_mouse_pressed(ctx->mouse, MouseButtonStateLeft))
    {
        ui_activate(ctx, id);
        ctx->drag_offset = sub_vec2(container->center, ctx->mouse.world);
    }
    else if(ui_is_active(ctx, id) && input_mouse_pressed(ctx->mouse, MouseButtonStateLeft))
    {
        container->x = ctx->mouse.world.x + ctx->drag_offset.x;
        container->y = ctx->mouse.world.y + ctx->drag_offset.y;
    }
    else if(ui_is_active(ctx, id) && input_mouse_released(ctx->mouse, MouseButtonStateLeft))
    {
        if(ui_active_time(ctx) < 200)
            b32_flip(is_expanded);

        ui_active_clear(ctx);
    }

    UIWindow result;
    result.header = rect_place_under(rect_from_wh(container->w, 30), header);
    result.is_expanded = *is_expanded;
    return result;
}

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
    if(style.background.color.a > 0 || style.background.border_color.a > 0)
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
    Circle handle = circle(vec2(x, slider_row.y), px(20));
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