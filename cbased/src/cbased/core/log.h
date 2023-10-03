#pragma once

#include "cbased/core/base.h"

typedef enum LoggerType {
  LoggerType_Info,
  LoggerType_Debug,
  LoggerType_Warn,
  LoggerType_Err,
  LoggerType_Fatal,
  LoggerType_Count,
} LoggerType;

#ifdef __cplusplus
extern "C" {
#endif

API void log_print(u32 type, const char *time, const char *file_path, u32 line,
                   const char *format, ...);

#ifdef __cplusplus
}
#endif

#define LOG_INFO(format, ...)                                                 \
  log_print(LoggerType_Info, __TIME__, __FILE__, __LINE__, format,            \
            ##__VA_ARGS__)

#define LOG_DEBUG(format, ...)                                                \
  log_print(LoggerType_Debug, __TIME__, __FILE__, __LINE__, format,           \
            ##__VA_ARGS__)

#define LOG_WARN(format, ...)                                                 \
  log_print(LoggerType_Warn, __TIME__, __FILE__, __LINE__, format,            \
            ##__VA_ARGS__)

#define LOG_ERR(format, ...)                                                  \
  log_print(LoggerType_Err, __TIME__, __FILE__, __LINE__, format,             \
            ##__VA_ARGS__);

#define LOG_FATAL(format, ...)                                                \
  log_print(LoggerType_Fatal, __TIME__, __FILE__, __LINE__, format,           \
            ##__VA_ARGS__);                                                   \
  assert(false)
