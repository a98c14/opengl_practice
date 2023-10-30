#pragma once

#include "stb_image.h"
#include "opus_graphics.h"
#include "opus_file.h"

internal Texture
texture_load_from_file(String texture_path, bool32 pixel_perfect, bool32 flip_vertical);