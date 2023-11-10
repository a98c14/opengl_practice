#pragma once
#include <core/defines.h>

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

typedef struct
{
    float32 last_frame;
    float32 current_frame;
    float32 dt;
} EngineTime;

internal EngineTime
engine_time_new();

internal EngineTime
engine_get_time(EngineTime prev);