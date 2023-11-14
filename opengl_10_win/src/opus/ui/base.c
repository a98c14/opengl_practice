#include "base.h"

internal UIContext* 
ui_context_new(Arena* arena, DrawContext* draw_context, Theme* theme)
{
    UIContext* ctx = arena_push_struct_zero_aligned(arena, UIContext, 16);
    ctx->frame_stack = arena_push_array_zero_aligned(arena, UIFrame, UI_FRAME_CAPACITY, 16);
    ctx->theme = theme;
    ctx->dc = draw_context;
    ctx->line_height = em(20);
    ctx->spacing = em(4);
    return ctx;
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
    // cursor = rect_shrink_f32(cursor, padding.x * 2); // left + right padding
    // cursor = rect_move(cursor, vec2(0, -padding.y)); // since we don't know the final height apply only top padding here
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
    Rect row = rect(0, 0, frame->cursor.w, ctx->line_height);
    row = rect_anchor(row, frame->cursor, ANCHOR_TL_TL);
    frame->cursor = rect_move(frame->cursor, vec2(0, -row.h-ctx->spacing));
    return row;
}

internal void
ui_frame_begin(UIContext* ctx, Vec2 pos, Vec2 size, Alignment alignment, Vec2 padding)
{
    UIFrame* frame = frame = ui_frame_new(ctx);
    frame->cursor = ui_cursor_new(ctx, pos, size.x);
    frame->base_alignment = alignment;
    frame->padding = padding;
    frame->base = frame->cursor;
    frame->cursor = rect_move(frame->cursor, vec2(0, -padding.y));
}

internal void
ui_frame_end(UIContext* ctx)
{
    UIFrame* frame = ui_active_frame(ctx);
    // add bottom padding remove the last element spacing (spacing only needs to be applied between the elements)
    frame->cursor = rect_move(frame->cursor, vec2(0, -frame->padding.y+ctx->spacing));
    Rect base_rect = rect_wh(frame->base.w, rect_bottom(frame->base)-rect_bottom(frame->cursor));
    base_rect = rect_anchor(base_rect, frame->cursor, ANCHOR_BL_BL);
    draw_rect(ctx->dc, base_rect, 0, 0, ctx->theme->rect_view);
    ctx->frame_count--;
}

internal void
ui_rect_basic(UIContext* ctx)
{
    UIFrame* frame = ui_active_frame(ctx);
    Rect row = rect(0, 0, frame->cursor.w, ctx->line_height);
    row = rect_anchor(row, frame->cursor, ANCHOR_TL_TL);
    draw_rect(ctx->dc, row, 0, 1, ctx->theme->rect_debug);
    frame->cursor = rect_move(frame->cursor, vec2(0, -row.h-ctx->spacing));
}

internal void
ui_window_begin(UIContext* ctx, String name, Vec2 pos, Vec2 size, Alignment alignment, Vec2 padding)
{
    UIFrame* frame = frame = ui_frame_new(ctx);
    frame->cursor = ui_cursor_new(ctx, pos, size.x);
    frame->base_alignment = alignment;
    frame->padding = padding;

    /* draw header */
    Rect header = rect_wh(frame->cursor.w, em(20));
    header = rect_anchor(header, frame->cursor, ANCHOR_TL_TL);
    draw_rect(ctx->dc, header, 0, 0, ctx->theme->rect_header);
    draw_text(ctx->dc, rect_cl(header), name, AlignmentLeft, ctx->theme->font_window_header);
    frame->cursor = rect_move(frame->cursor, vec2(0, -header.h));

    /* init root container */
    frame->base = frame->cursor;
    frame->cursor = rect_move(frame->cursor, vec2(0, -padding.y));
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
    Rect inner_row = rect_shrink(row, vec2(frame->padding.x*2, 0));
    draw_text(ctx->dc, rect_relative(inner_row, alignment), str, alignment, ctx->theme->font_default);
    debug_ui_rect(ctx->dc, row, 0, 1, ctx->theme->rect_debug);
}