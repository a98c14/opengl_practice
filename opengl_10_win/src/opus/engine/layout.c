#pragma once

#include "layout.h"

internal Rect
rect_aligned(float32 x, float32 y, float32 w, float32 h, Alignment alignment)
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
rect_align(Rect rect, Alignment alignment)
{
	rect.x += rect.w * AlignmentMultiplierX[alignment];
	rect.y += rect.h * AlignmentMultiplierY[alignment];
	return rect;
}

internal Rect
rect_anchor(Rect child, Rect parent, Anchor anchor)
{
	Rect result = rect_align(child, anchor.child);
	result.x = result.x + parent.x + AnchorMultiplierX[anchor.parent] * parent.w;
	result.y = result.y + parent.y + AnchorMultiplierY[anchor.parent] * parent.h;
	return result;
}

internal Vec2
rect_relative(Rect rect, Alignment alignment)
{
	Vec2 result = rect.center;
	result.x = rect.x - AlignmentMultiplierX[alignment] * rect.w;
	result.y = rect.y - AlignmentMultiplierY[alignment] * rect.h;
	return result;
}

internal Rect
rect_expand_f32(Rect rect, float32 v)
{
	rect.w = max(0, rect.w + v);
	rect.h = max(0, rect.h + v);
	return rect;
}

internal Rect
rect_expand(Rect rect, Vec2 v)
{
	rect.w = max(0, rect.w + v.x);
	rect.h = max(0, rect.h + v.y);
	return rect;
}

internal Rect
rect_shrink_f32(Rect rect, float32 v)
{
	return rect_expand_f32(rect, -v);
}

internal Rect
rect_shrink(Rect rect, Vec2 v)
{
	return rect_expand(rect, mul_vec2_f32(v, -1));
}

internal Rect
rect_move(Rect rect, Vec2 v)
{
	Rect result = rect;
	result.x += v.x;
	result.y += v.y;
	return result;
}