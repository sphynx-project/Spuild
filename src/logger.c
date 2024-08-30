#include "logger.h"
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static const char *log_level_to_str(log_level_t level)
{
    switch (level)
    {
    case LOG_LEVEL_DEBUG:
        return "DEBUG";
    case LOG_LEVEL_INFO:
        return "INFO";
    case LOG_LEVEL_WARNING:
        return "WARNING";
    case LOG_LEVEL_ERROR:
        return "ERROR";
    case LOG_LEVEL_FATAL:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
}

static const char *log_level_color(log_level_t level)
{
    switch (level)
    {
    case LOG_LEVEL_DEBUG:
        return LOG_COLOR_DEBUG;
    case LOG_LEVEL_INFO:
        return LOG_COLOR_INFO;
    case LOG_LEVEL_WARNING:
        return LOG_COLOR_WARNING;
    case LOG_LEVEL_ERROR:
        return LOG_COLOR_ERROR;
    case LOG_LEVEL_FATAL:
        return LOG_COLOR_FATAL;
    default:
        return LOG_COLOR_RESET;
    }
}

logger_ctx *logger_init(const char *scope, log_level_t min_level)
{
    logger_ctx *ctx = malloc(sizeof(logger_ctx));
    if (ctx == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    ctx->scope = strdup(scope);
    if (ctx->scope == NULL)
    {
        perror("strdup");
        free(ctx);
        exit(EXIT_FAILURE);
    }
    ctx->min_level = min_level;
    return ctx;
}

void log_message(logger_ctx *ctx, log_level_t level, const char *file, int line, const char *fmt, ...)
{
    if (level < ctx->min_level)
        return;

    time_t t;
    struct tm *tm_info;
    char time_buf[26];
    va_list args;

    time(&t);
    tm_info = localtime(&t);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(stdout, "[%s] [%-10s] ", time_buf, log_level_to_str(level));

    fprintf(stdout, "[%-8s] ", ctx->scope);
    fprintf(stdout, "[%s:%-8d] ", file, line);
    fprintf(stdout, "%s", log_level_color(level));

    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    fprintf(stdout, "%s\n", LOG_COLOR_RESET);
}

void logger_free(logger_ctx *ctx)
{
    if (ctx != NULL)
    {
        free((char *)ctx->scope);
        free(ctx);
    }
}
