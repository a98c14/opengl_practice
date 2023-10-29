#pragma once
#include "opus_base.h"
#include "opus_memory.h"

#define string(s) (String){(char*)s, lengthof(s)}

typedef struct
{
    char* value;
    uint64 length;
} String;

/* Functions */
internal String 
string_new(Arena* arena, uint64 length);

internal String
string_null();