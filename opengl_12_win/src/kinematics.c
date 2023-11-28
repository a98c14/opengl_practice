#include "kinematics.h"

internal void 
draw_joint(DrawContext* dc, Vec2 position, float32 rotation, float32 radius)
{
    // draw_line_fixed(dc, position, rotation, Color color, float32 thickness)
    draw_circle(dc, position, radius, ColorWhite);
}

internal void 
draw_arm(DrawContext* dc, Vec2 position, float32 rotation,float32 length)
{

}
