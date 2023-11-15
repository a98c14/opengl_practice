#pragma once

#include <core/defines.h>
#include <engine/draw.h>
#include "base.h"

internal Rect
ui_container(UIContext* ctx, Rect container, StyleContainer style);

internal void
ui_label(UIContext* ctx, Rect container, String str, StyleLabel style);

internal bool32
ui_slider(UIContext* ctx, String label, Range range, float32* value);

internal bool32
ui_button(UIContext* ctx, String label);

internal void
ui_grid_begin(UIContext* ctx, Rect rect, uint32 rows, uint32 columns);
