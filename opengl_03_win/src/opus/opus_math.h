#pragma once
#include <math.h>
#include "opus_base.h"

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

/* Basic Operations */
internal Mat3 mul_mat3(Mat3 a, Mat3 b);
internal Vec3 mul_mat3_vec3(Mat3 m, Vec3 v);
internal Mat4 mul_mat4(Mat4 a, Mat4 b);

/* Matrix Operations*/
internal Mat2 mat2_identity(void);
internal Mat2 mat2_rotation(float angle);
internal Mat2 mat2_transpose(Mat2 m);

internal Mat3 mat3_identity(void);
internal Mat3 mat3_rotation(float angle);
internal Mat3 mat3_rotate(Mat3 m, float32 angle);
internal Mat3 mat3_transpose(Mat3 m);

internal Mat4 mat4_identity(void);
internal Mat4 mat4_rotation(float angle);
internal Mat4 mat4_rotate_z(Mat4 m, float32 angle);
internal Mat4 mat4_ortho(float32 width, float32 height, float32 near, float32 far);

internal Vec4 linear_combine_v4_m4(Vec4 v, Mat4 m);