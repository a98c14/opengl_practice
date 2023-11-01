#pragma once

#include "opus_math.h"

internal Mat4
mat4_mvp(Mat4 model, Mat4 view, Mat4 projection);

/* Transforms */
internal Mat4
transform_quad(Vec2 position, Vec2 scale, float32 rotation);

internal Mat4
transform_line(Vec2 start, Vec2 end, float32 thickness);