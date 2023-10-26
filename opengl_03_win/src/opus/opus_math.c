#include "opus_math.h"

/* Basic Operations */
internal Vec3
mul_mat3_vec3(Mat3 m, Vec3 v)
{
    Vec3 result;
    result.x = v.elements[0] * m.columns[0].x;
    result.y = v.elements[0] * m.columns[0].y;
    result.z = v.elements[0] * m.columns[0].z;

    result.x += v.elements[1] * m.columns[1].x;
    result.y += v.elements[1] * m.columns[1].y;
    result.z += v.elements[1] * m.columns[1].z;

    result.x += v.elements[2] * m.columns[2].x;
    result.y += v.elements[2] * m.columns[2].y;
    result.z += v.elements[2] * m.columns[2].z;
    return result;
}

internal Mat3
mul_mat3(Mat3 a, Mat3 b)
{
    Mat3 result;
    result.columns[0] = mul_mat3_vec3(a, b.columns[0]);
    result.columns[1] = mul_mat3_vec3(a, b.columns[1]);
    result.columns[2] = mul_mat3_vec3(a, b.columns[2]);
    return result;
}

internal Mat4 
mul_mat4(Mat4 a, Mat4 b)
{
    Mat4 result;
    result.columns[0] = linear_combine_v4_m4(b.columns[0], a);
    result.columns[1] = linear_combine_v4_m4(b.columns[1], a);
    result.columns[2] = linear_combine_v4_m4(b.columns[2], a);
    result.columns[3] = linear_combine_v4_m4(b.columns[3], a);
    return result;
}

/* Matrix Operations */
internal Mat2
mat2_identity(void)
{
    Mat2 result = {0};
    result.m[0][0] = 1;
    result.m[1][1] = 1;
    return result;
}

internal Mat2
mat2_rotation(float angle)
{
    float32 radian = angle * PI_FLOAT32 / 180.0;
    float32 cosx = (float32)cos(radian);
    float32 sinx = (float32)sin(radian);

    Mat2 result = {0};
    result.m[0][0] = cosx;
    result.m[0][1] = sinx;
    result.m[1][0] = -sinx;
    result.m[1][1] = cosx;
    return result;
}

internal Mat2
mat2_transpose(Mat2 m)
{
    Mat2 result = m;
    result.m[0][1] = m.m[1][0];
    result.m[1][0] = m.m[0][1];
    return result;
}


internal Mat3
mat3_identity(void)
{
    Mat3 result = {0};
    result.m[0][0] = 1;
    result.m[1][1] = 1;
    result.m[2][2] = 1;
    return result;
}

/* Rotates around Z axis. */
internal Mat3
mat3_rotation(float angle)
{
    float32 radian = angle * PI_FLOAT32 / 180.0;
    float32 cosx = (float32)cos(radian);
    float32 sinx = (float32)sin(radian);

    Mat3 result = {0};
    result.m[0][0] = cosx;
    result.m[0][1] = sinx;
    result.m[1][0] = -sinx;
    result.m[1][1] = cosx;
    result.m[2][2] = 1;
    return result;
}

internal Mat3
mat3_rotate(Mat3 m, float32 angle)
{
    Mat3 rotation = mat3_rotation(angle);
    Mat3 result = mul_mat3(m, rotation);
    return result;
}

internal Mat3
mat3_transpose(Mat3 m)
{
    Mat3 result = m;
    result.m[0][1] = m.m[1][0];
    result.m[0][2] = m.m[2][0];
    result.m[1][0] = m.m[0][1];
    result.m[1][2] = m.m[2][1];
    result.m[2][1] = m.m[1][2];
    result.m[2][0] = m.m[0][2];
    return result;
}

internal Mat4
mat4_identity()
{
    Mat4 result = {0};
    result.m[0][0] = 1;
    result.m[1][1] = 1;
    result.m[2][2] = 1;
    result.m[3][3] = 1;
    return result;
}

internal Mat4
mat4_rotation(float32 angle)
{
    float32 radian = angle * PI_FLOAT32 / 180.0;
    float32 cosx = (float32)cosf(radian);
    float32 sinx = (float32)sinf(radian);
    Mat4 result = mat4_identity();
    result.m[0][0] =  cosx;
    result.m[0][1] =  sinx;
    result.m[1][0] = -sinx;
    result.m[1][1] =  cosx;
    return result;
}

internal Mat4
mat4_rotate_z(Mat4 m, float32 angle)
{
    Mat4 rotation = mat4_rotation(angle);
    Mat4 result = mul_mat4(m, rotation);
    return result;
}

internal Mat4
mat4_ortho(float32 width, float32 height, float32 near, float32 far)
{
    float32 right  =  width  / 2.0;
    float32 left   = -width  / 2.0;
    float32 top    =  height / 2.0;
    float32 bottom = -height / 2.0;
    
    Mat4 result = {0};
    result.m[0][0] =  2.0 / (right - left);
    result.m[1][1] =  2.0 / (top - bottom);
    result.m[2][2] = -2.0 / (far - near);
    result.m[0][3] =  -(right + left) / (right - left);
    result.m[1][3] =  -(top + bottom) / (top - bottom);
    result.m[2][3] =  -(far + near) / (far - near);
    result.m[3][3] = 1.0;
    return result;
}

internal Vec4
linear_combine_v4_m4(Vec4 v, Mat4 m)
{
    Vec4 result;
#ifdef OPUS_USE_SSE
    result.SSE = _mm_mul_ps(_mm_shuffle_ps(v.SSE, v.SSE, 0x00), m.columns[0].SSE);
    result.SSE = _mm_add_ps(result.SSE, _mm_mul_ps(_mm_shuffle_ps(v.SSE, v.SSE, 0x55), m.columns[1].SSE));
    result.SSE = _mm_add_ps(result.SSE, _mm_mul_ps(_mm_shuffle_ps(v.SSE, v.SSE, 0xaa), m.columns[2].SSE));
    result.SSE = _mm_add_ps(result.SSE, _mm_mul_ps(_mm_shuffle_ps(v.SSE, v.SSE, 0xff), m.columns[3].SSE));
#else
    result.x = v.elements[0] * m.columns[0].x;
    result.y = v.elements[0] * m.columns[0].y;
    result.z = v.elements[0] * m.columns[0].z;
    result.w = v.elements[0] * m.columns[0].w;

    result.x += v.elements[1] * m.columns[1].x;
    result.y += v.elements[1] * m.columns[1].y;
    result.z += v.elements[1] * m.columns[1].z;
    result.w += v.elements[1] * m.columns[1].w;

    result.x += v.elements[2] * m.columns[2].x;
    result.y += v.elements[2] * m.columns[2].y;
    result.z += v.elements[2] * m.columns[2].z;
    result.w += v.elements[2] * m.columns[2].w;

    result.x += v.elements[3] * m.columns[3].x;
    result.y += v.elements[3] * m.columns[3].y;
    result.z += v.elements[3] * m.columns[3].z;
    result.w += v.elements[3] * m.columns[3].w;
#endif
    return result;
}
