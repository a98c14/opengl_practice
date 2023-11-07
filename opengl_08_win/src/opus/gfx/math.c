#include "math.h"

internal Mat4
mat4_mvp(Mat4 model, Mat4 view, Mat4 projection)
{
    Mat4 result;
    result = mul_mat4(view, model);
    result = mul_mat4(projection, result);
    return result;
}

/* Transforms */
internal Mat4
transform_quad(Vec2 position, Vec2 scale, float32 rotation)
{
    Mat4 translation_mat = mat4_translation(vec3_xy_z(position, 0));
    Mat4 rotation_mat = mat4_rotation(rotation);
    Mat4 scale_mat = mat4_scale(vec3_xy_z(scale, 0));
    return mat4_transform(translation_mat, rotation_mat, scale_mat);
}

internal Mat4
transform_quad_aligned(Vec2 position, Vec2 scale)
{
    Mat4 result = mat4_identity();
    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = 0;
    result.m[3][0] = position.x;
    result.m[3][1] = position.y;
    result.m[3][2] = 0;
    return result;
}

internal Mat4
transform_line(Vec2 start, Vec2 end, float32 thickness)
{
    Vec2 center = lerp_vec2(start, end, 0.5f);
    float32 dist = dist_vec2(end, start);
    float32 angle = angle_vec2(sub_vec2(end, start));
    Mat4 translation = mat4_translation(vec3_xy_z(center, 0));
    Mat4 rotation = mat4_rotation(angle);
    Mat4 scale = mat4_scale(vec3(dist, thickness, 0));
    return mat4_transform(translation, rotation, scale);
}