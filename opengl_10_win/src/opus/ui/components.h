#pragma once

#include <core/defines.h>
#include <engine/draw.h>
#include "base.h"

typedef struct
{
    Rect container;
    bool32 is_expanded;
} UIWindow;

internal Rect
ui_container(UIContext* ctx, Rect container, StyleContainer style);

internal UIWindow
ui_window(UIContext* ctx, Rect container, String name, bool32* is_expanded, StyleWindow style);

internal void
ui_label(UIContext* ctx, Rect container, String str, StyleLabel style);

internal bool32
ui_slider(UIContext* ctx, String label, Range range, float32* value);

internal bool32
ui_button(UIContext* ctx, String label);