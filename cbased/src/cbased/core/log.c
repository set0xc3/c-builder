#include "log.h"

global const char *logger_types_string[LoggerType_Count] = {
  "INFO", "DEBUG", "WARN", "ERR", "FATAL",
};

api void
log_print(u32 type, const char *time, const char *file_path, u32 line,
          const char *format, ...)
{
  va_list args;
  va_start(args, format);

  char       *buffer       = NULL;
  FILE       *stream       = NULL;
  const char *prefix       = "";
  const u32   prefix_width = 6;

  stream = stdout;
  prefix = logger_types_string[type];

  va_list args_copy;
  va_copy(args_copy, args);
  u64 buffer_size = vsnprintf(NULL, 0, format, args_copy);
  va_end(args_copy);

  buffer = realloc(buffer, buffer_size + 1);
  vsnprintf(buffer, buffer_size + 1, format, args);

  fprintf(stream, "[%s][%s][%s:%d]", time, prefix, file_path, line);
  fprintf(stream, " %s %s", "-", buffer);

  va_end(args);

  free(buffer);
}
