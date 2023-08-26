#pragma once

#include "defines.h"

typedef enum LoggerType
{
    LoggerType_Info,
    LoggerType_Debug,
    LoggerType_Warning,
    LoggerType_Error,
    LoggerType_Count,
} LoggerType;

API void log_print(u32 type, const char *time, const char *file_path, u32 line,
                   const char *format, ...);

#define LOG_INFO(format, ...)                                                 \
    log_print(LoggerType_Info, __TIME__, __FILE__, __LINE__, format,          \
              ##__VA_ARGS__)
#define LOG_DEBUG(format, ...)                                                \
    log_print(LoggerType_Debug, __TIME__, __FILE__, __LINE__, format,         \
              ##__VA_ARGS__)
#define LOG_WARRNING(format, ...)                                             \
    log_print(LoggerType_Warning, __TIME__, __FILE__, __LINE__, format,       \
              ##__VA_ARGS__)
#define LOG_ERROR(format, ...)                                                \
    log_print(LoggerType_Error, __TIME__, __FILE__, __LINE__, format,         \
              ##__VA_ARGS__)
