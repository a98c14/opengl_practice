#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "opus_base.h"
#include "opus_memory.h"
#include "opus_string.h"

internal String
file_read_all_as_string(Arena* arena, String path);