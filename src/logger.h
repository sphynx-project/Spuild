#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef enum
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} log_level_t;

typedef struct
{
    const char *scope;
    log_level_t min_level;
} logger_ctx;

#define LOG_COLOR_RESET "\033[0m"
#define LOG_COLOR_DEBUG "\033[34m"
#define LOG_COLOR_INFO "\033[32m"
#define LOG_COLOR_WARNING "\033[33m"
#define LOG_COLOR_ERROR "\033[31m"
#define LOG_COLOR_FATAL "\033[41m"

#define log_debug(ctx, fmt, ...) log_message((ctx), LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(ctx, fmt, ...) log_message((ctx), LOG_LEVEL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warning(ctx, fmt, ...) log_message((ctx), LOG_LEVEL_WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_error(ctx, fmt, ...) log_message((ctx), LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_fatal(ctx, fmt, ...) log_message((ctx), LOG_LEVEL_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

logger_ctx *logger_init(const char *scope, log_level_t min_level);
void log_message(logger_ctx *ctx, log_level_t level, const char *file, int line, const char *fmt, ...);
void logger_free(logger_ctx *ctx);

#endif
