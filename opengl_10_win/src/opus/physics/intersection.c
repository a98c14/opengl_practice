#include "intersection.h"

internal bool
intersects_rect_point(Rect a, Vec2 b)
{
    Vec2 bl = rect_bl(a);
    Vec2 tr = rect_tr(a);
    
    return !(b.x < bl.x || b.y < bl.y || b.x > tr.x || b.y > tr.y);
}