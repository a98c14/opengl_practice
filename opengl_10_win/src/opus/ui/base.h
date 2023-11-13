#pragma once

#include <core/defines.h>
#include <engine/draw.h>
#include <theme/base.h>

#define UI_FRAME_CAPACITY 256
#define DEFAULT_LINE_HEIGHT 20

typedef uint8 FrameIndex;

typedef struct
{
    int16 owner;
    int16 item;
} UIID;

typedef struct
{
    UIID id;
    Rect frame;
} UIView;

typedef struct
{
	Rect base;
	Rect cursor;
	Vec2 padding;
    Alignment base_alignment;
} UIFrame;

typedef struct
{
    DrawContext* dc;
    Theme* theme;
    float32 line_height;
    float32 spacing;
    
    /* UI State */
    UIID active;
    UIID hot;

    uint8 frame_count;
    UIFrame* frame_stack;
} UIContext;

internal UIContext* 
ui_context_new(Arena* arena, DrawContext* draw_context, Theme* theme);

internal UIFrame*
ui_active_frame(UIContext* ctx);

internal void
ui_frame_begin(UIContext* ctx, Vec2 pos, Vec2 size, Alignment alignment, Vec2 padding);

internal void
ui_frame_end(UIContext* ctx);

internal void
ui_text(UIContext* ctx, String str);