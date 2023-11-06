#pragma once

#include "opus_base.h"
#include "opus_graphics.h"

internal void
renderer_enable_debug();

internal void
renderer_debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);