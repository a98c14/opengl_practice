#pragma once

#include <core/defines.h>
#include <physics/intersection.h>
#include <engine/time.h>
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
    EngineTime time;
    InputMouse mouse;

    /* State */
    UIID active;
    UIID hot;

    float32 activation_time;
    Vec2 drag_start;
    Vec2 drag_offset;

    // TODO: move these to someplace else
    Arena* frame_arena;
    Theme* theme;
    uint8 frame_count;
    UIFrame* frame_stack;
} UIContext;


internal UIID
uuid_new(int16 id, int16 owner);

internal UIID
uuid_null();

internal bool32
uuid_is_null(UIID id);

internal UIContext*
ui_context_new(Arena* arena, Arena* frame_arena, DrawContext* draw_context, Theme* theme);

internal bool32
ui_is_active(UIContext* ctx, UIID id);

internal bool32
ui_is_active_any(UIContext* ctx);

internal bool32 
ui_is_free(UIContext* ctx);

internal void
ui_activate(UIContext* ctx, UIID id);

internal void
ui_active_clear(UIContext* ctx);

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

internal bool32
ui_window_begin(UIContext* ctx, String name, Vec2* pos, Vec2 size, bool32* is_enabled);

internal void
ui_window_end(UIContext* ctx);

internal void
ui_text(UIContext* ctx, String str);