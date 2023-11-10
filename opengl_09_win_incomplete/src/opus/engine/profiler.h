#pragma once
#include <core/defines.h>
#include <core/strings.h>

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

typedef struct
{
    String name;
    float64 start;
    float64 end;
} Profiler;

internal Profiler
profiler_begin(String name);

internal void
profiler_end(Profiler* profiler);