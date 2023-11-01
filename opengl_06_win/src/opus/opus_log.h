#pragma once

#include "opus_base.h"
#include <stdarg.h>
#include <string.h>

internal void log_error(const char* message, ...); 
internal void log_info(const char* message, ...); 
internal void log_debug(const char* message, ...);

