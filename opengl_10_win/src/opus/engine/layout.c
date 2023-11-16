#pragma once

#include <core/defines.h>
#include <core/asserts.h>
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

internal Rect
rect_place(Rect child, Rect parent, Anchor anchor)
{
	Rect result = rect_wh(child.w, child.h);
	result = rect_align(result, anchor.child);
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

internal LayoutGrid
layout_grid(Rect container, int32 columns, int32 rows, Vec2 padding)
{
	LayoutGrid result = {0};
	result.base_container = rect_shrink(container, mul_vec2_f32(padding, 2));
	result.padding = padding;
	result.rows = rows;
	result.columns = columns;
	result.cell_size = vec2(result.base_container.w / columns, result.base_container.h / rows);
	return result;
}

internal Rect
layout_grid_container(LayoutGrid layout)
{
	return rect_expand(layout.base_container, layout.padding);
}

internal Rect
layout_grid_cell(LayoutGrid layout, int32 column, int32 row)
{
	xassert(column < layout.columns && row < layout.rows, "invalid column or row provided for grid layout");
	Vec2 tl = rect_tl(layout.base_container);
	float32 x = tl.x + column * layout.cell_size.w + layout.cell_size.w / 2;
	float32 y = tl.y - row * layout.cell_size.h - layout.cell_size.h / 2;
	return rect(x, y, layout.cell_size.w, layout.cell_size.h);
}

internal Rect
layout_grid_multicell(LayoutGrid layout, int32 column, int32 row, int32 column_count, int32 row_count)
{
	xassert(column + column_count <= layout.columns && row + row_count <= layout.rows, "invalid column or row provided for grid layout");
	float32 w = layout.cell_size.w * column_count;
	float32 h = layout.cell_size.h * row_count;
	Vec2 tl = rect_tl(layout.base_container);
	float32 x = tl.x + column * layout.cell_size.w + w / 2;
	float32 y = tl.y + row * layout.cell_size.h + h / 2;
	return rect(x, y, w, h);
}