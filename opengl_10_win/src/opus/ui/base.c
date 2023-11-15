#include "base.h"

internal UIID
uuid_new(int16 id, int16 owner)
{
    UIID result;
    result.item = id;
    result.owner = owner;
    return result;
}

internal UIID
uuid_null()
{
    UIID result;
    result.item = -1;
    result.owner = -1;
    return result;
}

internal bool32
uuid_is_null(UIID id)
{
    return id.item == -1 && id.owner == -1;
}

internal UIContext*
ui_context_new(Arena* arena, Arena* frame_arena, DrawContext* draw_context, Theme* theme)
{
    UIContext* ctx = arena_push_struct_zero_aligned(arena, UIContext, 16);
    ctx->frame_stack = arena_push_array_zero_aligned(arena, UIFrame, UI_FRAME_CAPACITY, 16);
    ctx->theme = theme;
    ctx->frame_arena = frame_arena;
    ctx->dc = draw_context;
    ctx->active = uuid_null();
    return ctx;
}

internal bool32
ui_is_active_any(UIContext* ctx)
{
    return ctx->active.item >= 0 || ctx->active.owner >= 0;
}

internal bool32 
ui_is_free(UIContext* ctx)
{
    return !ui_is_active_any(ctx);
}

internal bool32
ui_is_active(UIContext* ctx, UIID id)
{
    return ctx->active.item == id.item && ctx->active.owner == id.owner;
}

internal void
ui_activate(UIContext* ctx, UIID id)
{
    ctx->active = id;
    ctx->activation_time = ctx->time.current_frame;
}

internal void
ui_active_clear(UIContext* ctx)
{
    ctx->activation_time = 0;
    ctx->active = uuid_null();
    ctx->drag_offset = vec2_zero();
}

internal float32
ui_active_time(UIContext* ctx)
{
    return ctx->time.current_frame - ctx->activation_time;
}

internal bool32
ui_is_hot(UIContext* ctx, UIID id)
{
    return ctx->hot.item == id.item && ctx->hot.owner == id.owner;
}

internal UIFrame*
ui_active_frame(UIContext* ctx)
{
    xassert(ctx->frame_count - 1 >= 0 && ctx->frame_count - 1 < UI_FRAME_CAPACITY, "ui frame index is out of bounds");
    return &ctx->frame_stack[ctx->frame_count - 1];
}

internal Rect
ui_cursor_new(UIContext* ctx, Vec2 pos, float width)
{
    Rect cursor;
    cursor = rect_align(rect(pos.x, pos.y, width, 0), AlignmentBottomLeft);
    return cursor;
}

internal UIFrame*
ui_frame_new(UIContext* ctx)
{
    FrameIndex frame_index = ctx->frame_count;
    ctx->frame_count++;
    return &ctx->frame_stack[frame_index];
}

internal Rect
ui_row(UIContext* ctx, UIFrame* frame)
{
    Rect row = rect(0, 0, frame->cursor.w, ctx->theme->line_height);
    row = rect_anchor(row, frame->cursor, ANCHOR_TL_TL);
    frame->cursor = rect_move(frame->cursor, vec2(0, -row.h-ctx->theme->spacing));
    return row;
}

internal void
ui_frame_begin(UIContext* ctx, Vec2 pos, Vec2 size)
{
    UIFrame* frame = frame = ui_frame_new(ctx);
    frame->cursor = ui_cursor_new(ctx, pos, size.x);
    frame->base = frame->cursor;
    frame->cursor = rect_move(frame->cursor, vec2(0, -ctx->theme->padding.y));
}

internal void
ui_frame_end(UIContext* ctx)
{
    UIFrame* frame = ui_active_frame(ctx);
    // add bottom padding remove the last element spacing (spacing only needs to be applied between the elements)
    frame->cursor = rect_move(frame->cursor, vec2(0, -ctx->theme->padding.y+ctx->theme->spacing));
    Rect base_rect = rect_wh(frame->base.w, rect_bottom(frame->base)-rect_bottom(frame->cursor));
    base_rect = rect_anchor(base_rect, frame->cursor, ANCHOR_BL_BL);
    draw_rect(ctx->dc, base_rect, 0, 0, ctx->theme->rect_view);
    ctx->frame_count--;
}

internal void
ui_rect_basic(UIContext* ctx)
{
    UIFrame* frame = ui_active_frame(ctx);
    Rect row = rect(0, 0, frame->cursor.w, ctx->theme->line_height);
    row = rect_anchor(row, frame->cursor, ANCHOR_TL_TL);
    draw_rect(ctx->dc, row, 0, 1, ctx->theme->rect_debug);
    frame->cursor = rect_move(frame->cursor, vec2(0, -row.h-ctx->theme->spacing));
}

internal bool32
ui_window_begin(UIContext* ctx, String name, Vec2* pos, Vec2 size, bool32* is_enabled)
{
    // TODO: use an actual id
    int32 name_hash = hash_string(name);
    UIID id = uuid_new(name_hash, 0);
    UIFrame* frame = frame = ui_frame_new(ctx);
    frame->cursor = ui_cursor_new(ctx, *pos, size.x);

    /* draw header */
    Rect header = rect_wh(frame->cursor.w, em(24));
    header = rect_anchor(header, frame->cursor, ANCHOR_TL_TL);
    bool32 hover = intersects_rect_point(header, ctx->mouse.world);
    if(!ui_is_active_any(ctx) && hover && input_mouse_pressed(ctx->mouse, MouseButtonStateLeft))
    {
        ui_activate(ctx, id);
        ctx->drag_offset = sub_vec2(rect_tl(header), ctx->mouse.world);
    }
    else if(ui_is_active(ctx, id) && input_mouse_pressed(ctx->mouse, MouseButtonStateLeft))
    {
        pos->x = ctx->mouse.world.x + ctx->drag_offset.x;
        pos->y = ctx->mouse.world.y + ctx->drag_offset.y;
    }
    else if(ui_is_active(ctx, id) && input_mouse_released(ctx->mouse, MouseButtonStateLeft))
    {
        if(ui_active_time(ctx) < 200)
            b32_flip(is_enabled);

        ui_active_clear(ctx);
    }

    StyleRect header_style = hover ? ctx->theme->rect_header_hover : ctx->theme->rect_header;
    draw_rect(ctx->dc, header, 0, 0, header_style);
    draw_text(ctx->dc, rect_cl(rect_shrink_f32(header, 4)), name, AlignmentLeft, ctx->theme->font_window_header);
    frame->cursor = rect_move(frame->cursor, vec2(0, -header.h));

    /* init root container */
    frame->base = frame->cursor;
    frame->cursor = rect_move(frame->cursor, vec2(0, -ctx->theme->padding.y));
    return *is_enabled;
}

internal void
ui_window_end(UIContext* ctx)
{
    ui_frame_end(ctx);
}

internal void
ui_text(UIContext* ctx, String str)
{
    UIFrame* frame = ui_active_frame(ctx);
    Rect row = ui_row(ctx, frame);

    Alignment alignment = AlignmentLeft;
    Rect inner_row = rect_shrink(row, vec2(ctx->theme->padding.x*2, 0));
    draw_text(ctx->dc, rect_relative(inner_row, alignment), str, alignment, ctx->theme->font_default);
    debug_ui_rect(ctx->dc, row, 0, 1, ctx->theme->rect_debug);
}
