#include "log.h"

internal void 
log_output(LogLevel level, const char* message, ...)
{
    const char* level_strings[6] = { "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    bool32 is_error = level <= LogLevelError;

    char out_buffer[LOG_MESSAGE_SIZE_LIMIT];
    memset(out_buffer, 0, sizeof(out_buffer));

    va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_buffer, LOG_MESSAGE_SIZE_LIMIT, message, arg_ptr);
    va_end(arg_ptr);

    char print_buffer[LOG_MESSAGE_SIZE_LIMIT];
    sprintf(print_buffer, "%s%s\n", level_strings[level], out_buffer);
    printf("%s", print_buffer);
}

internal bool32 log_initialize()
{
    return 1;
}

internal void
log_shutdown()
{
}

internal void
log_assertion_failure(const char* expr, const char* message, const char* file, int32 line)
{
    log_output(LogLevelFatal, "Assertion Failure: %s, message: '%s', in file %s, line: %d\n", expr, message, file, line);
}