#pragma once
#include <math.h>
#include "opus_base.h"

// NOTE(selim): Delete this if you don't wanna use SIMD
#define OPUS_USE_SSE
#ifdef OPUS_USE_SSE
# include <xmmintrin.h>
#endif

/* Constants */
global float32 EPSILON_FLOAT32 = 1.1920929e-7f;
global float32 PI_FLOAT32      = 3.14159265359f;
global float32 TAU_FLOAT32     = 6.28318530718f;

/* Vectors */
typedef struct { int32 x; int32 y; } IVec2;
typedef struct { int32 x; int32 y; int32 z; }  IVec3;
typedef struct { int32 x; int32 y; int32 z; int32 w; } IVec4;

typedef union 
{
    struct { float32 x; float32 y; };
    struct { float32 u; float32 v; };
    float32 elements[2];
} Vec2;

typedef union
{
    struct { float32 x; float32 y; float32 z; };
    struct { float32 r; float32 g; float32 b; };
    struct { Vec2 xy; float32 _ignored; };
    float32 elements[3];
} Vec3;

typedef union
{
    struct { float32 x; float32 y; float32 z; float32 w; };
    struct { float32 r; float32 g; float32 b; float32 a; };
    struct { Vec3 xyz; float32 _ignored0; };
    struct { Vec2 xy; float32 _ignored1; float32 _ignored2; };
    float32 elements[4];
#ifdef OPUS_USE_SSE
    __m128 SSE;
#endif
} Vec4;

/* Matrices */
typedef union
{
    float32 v[4];
    float32 m[2][2];
    Vec2 columns[2];
} Mat2;

typedef union
{
    float32 v[9];
    float32 m[3][3];
    Vec3 columns[3];
} Mat3;

typedef union
{
    float32 v[16];
    float32 m[4][4];
    Vec4 columns[4];
} Mat4;

/* Constructors */
internal Vec2 vec2(float32 x, float32 y);
internal Vec2 vec2_one();
internal Vec2 vec2_right();
internal Vec2 vec2_up();
internal Vec3 vec3(float32 x, float32 y, float32 z);
internal Vec3 vec3_xy_z(Vec2 xy, float32 z);
internal Vec4 vec4(float32 x, float32 y, float32 z, float32 w);

/* Basic Operations */
internal Vec2 add_vec2(Vec2 a, Vec2 b);
internal Vec4 add_vec4(Vec4 a, Vec4 b);
internal Vec2 sub_vec2(Vec2 a, Vec2 b);
internal Vec3 sub_vec3(Vec3 a, Vec3 b);
internal Vec4 sub_vec4(Vec4 a, Vec4 b);
internal Vec2 mul_vec2_f32(Vec2 a, float32 b);
internal Mat3 mul_mat3(Mat3 a, Mat3 b);
internal Vec3 mul_mat3_vec3(Mat3 m, Vec3 v);
internal Mat4 mul_mat4(Mat4 a, Mat4 b);

internal Vec2 lerp_vec2(Vec2 a, Vec2 b, float32 t);

/* Vector Operations */
internal float32 dot_vec2(Vec2 a, Vec2 b);
internal float32 dot_vec3(Vec3 a, Vec3 b);
internal float32 lensqr_vec2(Vec2 a);
internal float32 len_vec2(Vec2 a);
internal float32 dist_vec2(Vec2 a, Vec2 b);
internal float32 angle_between_vec2(Vec2 a, Vec2 b);

/* Matrix Operations*/
internal Mat2 mat2_identity(void);
internal Mat2 mat2_rotation(float angle);
internal Mat2 mat2_transpose(Mat2 m);

internal Mat3 mat3_identity(void);
internal Mat3 mat3_rotation(float angle);
internal Mat3 mat3_rotate(Mat3 m, float32 angle);
internal Mat3 mat3_transpose(Mat3 m);

internal Mat4 mat4_identity(void);
internal Mat4 mat4_translation(Vec3 v);
internal Mat4 mat4_scale(Vec3 v);
internal Mat4 mat4_rotation(float angle);
internal Mat4 mat4_transform(Mat4 translation, Mat4 rotation, Mat4 scale);
internal Mat4 mat4_ortho(float32 width, float32 height, float32 near, float32 far);

internal Vec4 linear_combine_v4_m4(Vec4 v, Mat4 m);