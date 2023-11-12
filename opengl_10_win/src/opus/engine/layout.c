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

internal Rect
rect_anchor(Rect parent, RectAlignmentType anchor, Rect rect)
{
	Rect result = rect;
	result.x = rect.x + parent.x + AnchorMultiplierX[anchor] * parent.w;
	result.y = rect.y + parent.y + AnchorMultiplierY[anchor] * parent.h;
	return result;
}