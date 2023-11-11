#pragma once

#include "layout.h"

internal Rect
rect_aligned(float32 x, float32 y, float32 w, float32 h, RectAlignmentType alignment)
{
	Rect result;
	result.x = x;
	result.y = y;
	result.w = w;
	result.h = h;
	result.x += result.w * AlignmentMultiplierX[alignment];
	result.y += result.h * AlignmentMultiplierY[alignment];
	return result;
}
