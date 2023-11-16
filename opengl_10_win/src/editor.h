#pragma once

#include <opus.h>

internal void
editor_draw_stats(Arena* persistent_arena, Arena* frame_arena, Renderer* renderer, UIContext* ctx, Theme* default_theme, EngineTime time, Profiler* main_frame, Profiler* update, Profiler* render, Rect screen, InputMouse mouse);