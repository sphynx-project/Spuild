#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

const char *color_to_string(LogColor color)
{
    switch (color)
    {
    case LOG_COLOR_RED:
        return "\x1b[31m";
    case LOG_COLOR_YELLOW:
        return "\x1b[33m";
    case LOG_COLOR_GREEN:
        return "\x1b[32m";
    case LOG_COLOR_CYAN:
        return "\x1b[36m";
    case LOG_COLOR_MAGENTA:
        return "\x1b[35m";
    case LOG_COLOR_BLUE:
        return "\x1b[34m";
    case LOG_COLOR_WHITE:
        return "\x1b[37m";
    case LOG_COLOR_RESET:
    default:
        return "\x1b[0m";
    }
}

const char *log_level_to_string(LogLevel level)
{
    switch (level)
    {
    case LOG_LEVEL_ERROR:
        return "ERROR";
    case LOG_LEVEL_WARN:
        return "WARN";
    case LOG_LEVEL_INFO:
        return "INFO";
    case LOG_LEVEL_DEBUG:
        return "DEBUG";
    case LOG_LEVEL_TRACE:
        return "TRACE";
    default:
        return "UNKNOWN";
    }
}

void get_current_time(char *buffer, size_t buffer_size, const char *format)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, buffer_size, format, tm_info);
}

unsigned long get_current_timestamp()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (unsigned long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

void logger_init(Logger *logger, LogLevel level, FILE *output, const char *scope, const char *name, const LogFormatKind *format, size_t format_length, const char *date_format, const LogColor *colors)
{
    logger->level = level;
    logger->output = output;
    logger->scope = scope ? strdup(scope) : NULL;
    logger->name = name ? strdup(name) : NULL;
    logger->file = NULL;
    logger->function = NULL;
    logger->line = 0;
    logger->thread_id = (unsigned long)pthread_self();
    logger->pid = (unsigned long)getpid();
    logger->date_format = date_format ? strdup(date_format) : "%Y-%m-%d %H:%M:%S";
    logger->start_time = get_current_timestamp();

    size_t len = format_length > 12 ? 12 : format_length;
    for (size_t i = 0; i < len; ++i)
    {
        logger->format[i] = format[i];
    }

    for (size_t i = len; i < 12; ++i)
    {
        logger->format[i] = LOG_FORMAT_MESSAGE;
    }

    logger->color[LOG_LEVEL_ERROR] = colors ? colors[LOG_LEVEL_ERROR] : LOG_COLOR_RED;
    logger->color[LOG_LEVEL_WARN] = colors ? colors[LOG_LEVEL_WARN] : LOG_COLOR_YELLOW;
    logger->color[LOG_LEVEL_INFO] = colors ? colors[LOG_LEVEL_INFO] : LOG_COLOR_GREEN;
    logger->color[LOG_LEVEL_DEBUG] = colors ? colors[LOG_LEVEL_DEBUG] : LOG_COLOR_CYAN;
    logger->color[LOG_LEVEL_TRACE] = colors ? colors[LOG_LEVEL_TRACE] : LOG_COLOR_BLUE;
}

void logger_set_level(Logger *logger, LogLevel level)
{
    logger->level = level;
}

void logger_set_scope(Logger *logger, const char *scope)
{
    if (logger->scope)
    {
        free((char *)logger->scope);
    }
    logger->scope = scope ? strdup(scope) : NULL;
}

void logger_set_name(Logger *logger, const char *name)
{
    if (logger->name)
    {
        free((char *)logger->name);
    }
    logger->name = name ? strdup(name) : NULL;
}

void logger_set_format(Logger *logger, const LogFormatKind *format, size_t format_length)
{
    size_t len = format_length > 12 ? 12 : format_length;
    for (size_t i = 0; i < len; ++i)
    {
        logger->format[i] = format[i];
    }

    for (size_t i = len; i < 12; ++i)
    {
        logger->format[i] = LOG_FORMAT_MESSAGE;
    }
}

void logger_set_date_format(Logger *logger, const char *date_format)
{
    if (logger->date_format)
    {
        free((char *)logger->date_format);
    }
    logger->date_format = date_format ? strdup(date_format) : "%Y-%m-%d %H:%M:%S";
}

void logger_set_colors(Logger *logger, const LogColor *colors)
{
    logger->color[LOG_LEVEL_ERROR] = colors ? colors[LOG_LEVEL_ERROR] : LOG_COLOR_RED;
    logger->color[LOG_LEVEL_WARN] = colors ? colors[LOG_LEVEL_WARN] : LOG_COLOR_YELLOW;
    logger->color[LOG_LEVEL_INFO] = colors ? colors[LOG_LEVEL_INFO] : LOG_COLOR_GREEN;
    logger->color[LOG_LEVEL_DEBUG] = colors ? colors[LOG_LEVEL_DEBUG] : LOG_COLOR_CYAN;
    logger->color[LOG_LEVEL_TRACE] = colors ? colors[LOG_LEVEL_TRACE] : LOG_COLOR_BLUE;
}

void logger_set_source_info(Logger *logger, const char *file, const char *function, unsigned int line, unsigned long thread_id, unsigned long pid)
{
    logger->file = file ? strdup(file) : NULL;
    logger->function = function ? strdup(function) : NULL;
    logger->line = line;
    logger->thread_id = thread_id;
    logger->pid = pid;
}

void log_message(Logger *logger, LogLevel level, const char *message, ...)
{
    if (level > logger->level)
    {
        return;
    }

    char time_buffer[100];
    get_current_time(time_buffer, sizeof(time_buffer), logger->date_format);

    fprintf(logger->output, "%s", color_to_string(logger->color[level]));

    va_list args;
    va_start(args, message);

    for (size_t i = 0; i < 12; ++i)
    {
        switch (logger->format[i])
        {
        case LOG_FORMAT_KIND:
            fprintf(logger->output, "[%s] ", log_level_to_string(level));
            break;
        case LOG_FORMAT_DATE:
            fprintf(logger->output, "[%s] ", time_buffer);
            break;
        case LOG_FORMAT_SCOPE:
            if (logger->scope)
            {
                fprintf(logger->output, "[%s] ", logger->scope);
            }
            break;
        case LOG_FORMAT_NAME:
            if (logger->name)
            {
                fprintf(logger->output, "[%s] ", logger->name);
            }
            break;
        case LOG_FORMAT_FILE:
            if (logger->file)
            {
                fprintf(logger->output, "[%s] ", logger->file);
            }
            break;
        case LOG_FORMAT_LINE:
            fprintf(logger->output, "[%u] ", logger->line);
            break;
        case LOG_FORMAT_FUNCTION:
            if (logger->function)
            {
                fprintf(logger->output, "[%s] ", logger->function);
            }
            break;
        case LOG_FORMAT_THREAD:
            fprintf(logger->output, "[%lu] ", logger->thread_id);
            break;
        case LOG_FORMAT_PID:
            fprintf(logger->output, "[%lu] ", logger->pid);
            break;
        case LOG_FORMAT_TIMESTAMP:
            fprintf(logger->output, "[%lu] ", get_current_timestamp());
            break;
        case LOG_FORMAT_ELAPSED_TIME:
            fprintf(logger->output, "[%lu ms] ", get_current_timestamp() - logger->start_time);
            break;
        case LOG_FORMAT_CUSTOM_TEXT:
            fprintf(logger->output, "[CUSTOM] ");
            break;
        case LOG_FORMAT_MESSAGE:
            vfprintf(logger->output, message, args);
            fprintf(logger->output, "\n");
            va_end(args);
            fprintf(logger->output, "%s", color_to_string(LOG_COLOR_RESET));
            return;
        }
    }

    va_end(args);
    fprintf(logger->output, "%s", color_to_string(LOG_COLOR_RESET));
    fprintf(logger->output, "\n");
}
