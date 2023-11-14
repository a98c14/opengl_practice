#pragma once

#include <core/defines.h>
#include <physics/intersection.h>
#include <engine/draw.h>
#include <engine/input.h>
#include <theme/base.h>
#include "debug.h"

#define UI_DEBUG 1
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
} UIFrame;

typedef struct
{
    DrawContext* dc;
    Theme* theme;

    /* Input */
    InputMouse mouse;

    /* Layout */
    uint8 frame_count;
    UIFrame* frame_stack;

    /* State */
    UIID active;
    UIID hot;

    Vec2 drag_offset;
} UIContext;

internal UIID
uuid_new(int16 id, int16 owner);

internal UIID
uuid_null();

internal bool32
uuid_is_null(UIID id);

internal UIContext*
ui_context_new(Arena* arena, DrawContext* draw_context, Theme* theme);

internal bool32
ui_is_active(UIContext* ctx, UIID id);

internal bool32
ui_is_hot(UIContext* ctx, UIID id);

internal UIFrame*
ui_active_frame(UIContext* ctx);

internal Rect
ui_cursor_new(UIContext* ctx, Vec2 pos, float width);

internal UIFrame*
ui_frame_new(UIContext* ctx);

/** pushes a new row to the given frame.
 *  moves the cursor and returns the available rect */
internal Rect
ui_row(UIContext* ctx, UIFrame* frame);


// TODO: move to componenents?
internal void
ui_frame_begin(UIContext* ctx, Vec2 pos, Vec2 size);

internal void
ui_frame_end(UIContext* ctx);

internal void
ui_window_begin(UIContext* ctx, String name, Vec2* pos, Vec2 size);

internal void
ui_window_end(UIContext* ctx);

internal void
ui_text(UIContext* ctx, String str);