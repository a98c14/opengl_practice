#pragma once
#include <stdint.h>

/* Defines */
#define global static
#define local static
#define internal static

/* Macros */
#define array_count(a) (sizeof(a)/sizeof(*(a)))
#define array_count_from_type(type, array_field) array_count(field_pointer(type, array_field))
#define alignof(x) (usize)_Alignof(x)
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)

#define ENABLE_ASSERT 1
#define stringify(value) #value
#define statement(statement) do{ statement }while(0)

#if ENABLE_ASSERT
#include <stdio.h>
#include <stdlib.h>
#define assert_break() (abort())
#define Assert(condition, message) statement(if (!(condition)){ printf("Assert(%s) | %s\n\tat %s:%d \n", #condition, message, __FILE__, __LINE__); assert_break(); })
#else
#define Assert(condition, message)
#endif

#define align_up_pow2(x, p) (((x) + (p) - 1)&~((p) - 1))

#define kb(x) ((x) << 10)
#define mb(x) ((x) << 20)
#define gb(x) ((x) << 30)
#define tb(x) ((x) << 40)

/* Primitive Types */
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32_t bool32;
typedef float float32;
typedef double float64;
typedef uintptr_t uptr;
typedef size_t usize;
typedef int32_t bool;