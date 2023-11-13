#include "base.h"

internal UIContext* 
ui_context_new(Arena* arena, DrawContext* draw_context, Theme* theme)
{
    UIContext* ctx = arena_push_struct_zero_aligned(arena, UIContext, 16);
    ctx->frame_stack = arena_push_array_zero_aligned(arena, UIFrame, UI_FRAME_CAPACITY, 16);
    ctx->theme = theme;
    ctx->dc = draw_context;
    ctx->line_height = 20 ;
    ctx->spacing = 2;
    return ctx;
}

internal UIFrame*
ui_active_frame(UIContext* ctx)
{
    xassert(ctx->frame_count - 1 >= 0 && ctx->frame_count - 1 < UI_FRAME_CAPACITY, "ui frame index is out of bounds");
    return &ctx->frame_stack[ctx->frame_count - 1];
}

internal void
ui_frame_begin(UIContext* ctx, Vec2 pos, Vec2 size, Alignment alignment, Vec2 padding)
{
    FrameIndex frame_index = ctx->frame_count;
    UIFrame* frame = &ctx->frame_stack[frame_index];
    ctx->frame_count++;
    Rect base = rect(pos.x, pos.y, size.x, size.y);
    frame->base = base;
    frame->cursor = rect_align(base, alignment);
    frame->base_alignment = alignment;
    frame->padding = padding;
    frame->cursor = rect_shrink(frame->cursor, padding.x * 2); // left + right padding
    frame->cursor = rect_move(frame->cursor, vec2(0, -padding.y)); // since we don't know the final height apply only top padding here
}

internal void
ui_frame_end(UIContext* ctx)
{
    UIFrame* frame = ui_active_frame(ctx);
    float32 base_container_height = rect_bottom(frame->base) - rect_bottom(frame->cursor) + 2.0f;
    base_container_height -= ctx->spacing; // this is subtracted so we don't add spacing for the last element
    Rect base_rect = rect(frame->base.x, frame->base.y, frame->base.w, base_container_height);
    base_rect = rect_align(base_rect, frame->base_alignment);
    draw_rect(ctx->dc, base_rect, 0, 0, ctx->theme->rect_default);
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