#pragma once

#include <core/defines.h>
#include <core/math.h>

typedef enum
{
	RectAlignmentTypeCenter,
	RectAlignmentTypeBottom,
	RectAlignmentTypeTop,
	RectAlignmentTypeRight,
	RectAlignmentTypeLeft,

	RectAlignmentTypeBottomLeft,
	RectAlignmentTypeBottomRight,
	RectAlignmentTypeTopLeft,
	RectAlignmentTypeTopRight,
	RectAlignmentTypeCount
} RectAlignmentType;

                                                                // Center,  Bottom,   Top,  Right,  Left,  BottomLeft,  BottomRight,  TopLeft,  TopRight
const float32 AlignmentMultiplierX[RectAlignmentTypeCount]     = {      0,       0,     0,   -0.5,   0.5,         0.5,         -0.5,      0.5,      -0.5 };
const float32 AlignmentMultiplierY[RectAlignmentTypeCount]     = {      0,     0.5,  -0.5,      0,     0,         0.5,          0.5,     -0.5,      -0.5 };
const float32 AnchorMultiplierX[RectAlignmentTypeCount]        = {      0,       0,     0,    0.5,  -0.5,        -0.5,          0.5,     -0.5,       0.5 };
const float32 AnchorMultiplierY[RectAlignmentTypeCount]        = {      0,    -0.5,   0.5,      0,     0,        -0.5,         -0.5,      0.5,       0.5 };

internal Rect
rect_aligned(float32 x, float32 y, float32 w, float32 h, RectAlignmentType alignment);