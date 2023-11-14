#pragma once

#include <core/defines.h>
#include <engine/draw.h>

#include "base.h"

internal bool32
ui_slider(UIContext* ctx, String label, Range range, float32* value);