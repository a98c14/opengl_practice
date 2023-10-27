#include "opus_string.h"

internal String 
string_new(Arena* arena, uint64 length)
{
    String s = {0};
    s.value = arena_push(arena, length); // +1 for null-terminator
    s.length = length;
    return s;
}

internal String
string_null()
{
    return (String){ .value = NULL, .length = -1 };
}