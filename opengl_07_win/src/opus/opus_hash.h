#pragma once

#include "opus_base.h"
#include "opus_string.h"

internal uint32 hash_uint64(uint64 value);
internal inline uint64 hash_chars(const char* buffer);
internal inline uint64 hash_string(String str);