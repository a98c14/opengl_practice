#include "profiler.h"

internal Profiler
profiler_begin(String name)
{
    Profiler result = {0};
    result.name = name;
    result.start = glfwGetTime()*1000;
    return result;
}

internal void
profiler_end(Profiler* profiler)
{
    profiler->end = glfwGetTime()*1000;
}