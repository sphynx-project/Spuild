#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

typedef enum
{
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
} LogLevel;

typedef enum
{
    LOG_FORMAT_KIND,
    LOG_FORMAT_DATE,
    LOG_FORMAT_SCOPE,
    LOG_FORMAT_NAME,
    LOG_FORMAT_FILE,
    LOG_FORMAT_LINE,
    LOG_FORMAT_FUNCTION,
    LOG_FORMAT_THREAD,
    LOG_FORMAT_PID,
    LOG_FORMAT_MESSAGE,
    LOG_FORMAT_TIMESTAMP,
    LOG_FORMAT_ELAPSED_TIME,
    LOG_FORMAT_CUSTOM_TEXT
} LogFormatKind;

typedef enum
{
    LOG_COLOR_RESET,
    LOG_COLOR_RED,
    LOG_COLOR_YELLOW,
    LOG_COLOR_GREEN,
    LOG_COLOR_CYAN,
    LOG_COLOR_MAGENTA,
    LOG_COLOR_BLUE,
    LOG_COLOR_WHITE
} LogColor;

typedef struct
{
    LogLevel level;
    FILE *output;
    const char *scope;
    const char *name;
    const char *file;
    const char *function;
    unsigned int line;
    unsigned long thread_id;
    unsigned long pid;
    LogFormatKind format[12];
    const char *date_format;
    LogColor color[5];
    unsigned long start_time;
} Logger;

void logger_init(Logger *logger, LogLevel level, FILE *output, const char *scope, const char *name, const LogFormatKind *format, size_t format_length, const char *date_format, const LogColor *colors);
void logger_set_level(Logger *logger, LogLevel level);
void logger_set_scope(Logger *logger, const char *scope);
void logger_set_name(Logger *logger, const char *name);
void logger_set_format(Logger *logger, const LogFormatKind *format, size_t format_length);
void logger_set_date_format(Logger *logger, const char *date_format);
void logger_set_colors(Logger *logger, const LogColor *colors);
void logger_set_source_info(Logger *logger, const char *file, const char *function, unsigned int line, unsigned long thread_id, unsigned long pid);
void log_message(Logger *logger, LogLevel level, const char *message, ...);

#define LOG_ERROR(logger, message, ...) log_message(logger, LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define LOG_WARN(logger, message, ...) log_message(logger, LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#define LOG_INFO(logger, message, ...) log_message(logger, LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#define LOG_DEBUG(logger, message, ...) log_message(logger, LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define LOG_TRACE(logger, message, ...) log_message(logger, LOG_LEVEL_TRACE, message, ##__VA_ARGS__)

#endif
