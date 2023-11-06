#include "opus_log.h"

#define DATE_BUFFER_SIZE 26

void log_format(const char* tag, const char* message, va_list args) 
{
    printf("[%s] ", tag);
    vprintf(message, args);
    printf("\n");
}

void log_error(const char* message, ...) 
{   va_list args;
    va_start(args, message);
    log_format("ERROR", message, args);
    va_end(args); 
}

void log_info(const char* message, ...) 
{
    va_list args;
    va_start(args, message);
    log_format("INFO", message, args);
    va_end(args);
}

void log_debug(const char* message, ...) 
{
    va_list args;
    va_start(args, message);
    log_format("DEBUG", message, args);
    va_end(args);
}
