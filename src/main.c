#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include "logger.h"

const char *argp_program_version = "spbuild 1.0";
const char *argp_program_bug_address = "sphynx@shittydev.com";

static char doc[] = "A modern build system, made to be easy";
static char args_doc[] = "[RECEPIE]";

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output", 0},
    {"file", 'f', "FILE", 0, "Specify the file path", 0},
    {0}};

struct arguments
{
    int verbose;
    char *file_path;
    char *recepie;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
    case 'v':
        arguments->verbose++;
        break;
    case 'f':
        arguments->file_path = strdup(arg);
        if (arguments->file_path == NULL)
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num == 0)
        {
            arguments->recepie = strdup(arg);
            if (arguments->recepie == NULL)
            {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            argp_usage(state);
        }
        break;
    case ARGP_KEY_END:
        if (state->arg_num == 0)
        {
            arguments->recepie = strdup("build");
            if (arguments->recepie == NULL)
            {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
        }
        if (arguments->file_path == NULL)
        {
            arguments->file_path = strdup(".spbuild");
            if (arguments->file_path == NULL)
            {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {
    options,
    parse_opt,
    args_doc,
    doc,
    NULL,
    NULL,
    0};

int main(int argc, char **argv)
{
    struct arguments arguments = {0};

    LogFormatKind format[] = {
        LOG_FORMAT_ELAPSED_TIME,
        LOG_FORMAT_DATE,
        LOG_FORMAT_KIND,
        LOG_FORMAT_SCOPE,
        LOG_FORMAT_NAME,
        LOG_FORMAT_MESSAGE};

    LogColor colors[] = {
        LOG_COLOR_RED,
        LOG_COLOR_YELLOW,
        LOG_COLOR_GREEN,
        LOG_COLOR_CYAN,
        LOG_COLOR_BLUE};

    Logger logger_main;

    logger_init(&logger_main, LOG_LEVEL_INFO, stdout, "main", "Entry", format, sizeof(format) / sizeof(format[0]), "%Y-%m-%d %H:%M:%S", colors);

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.verbose >= 2)
    {
        logger_set_level(&logger_main, LOG_LEVEL_TRACE);
    }
    else if (arguments.verbose == 1)
    {
        logger_set_level(&logger_main, LOG_LEVEL_DEBUG);
    }

    if (arguments.file_path)
    {
        LOG_TRACE(&logger_main, "File path: %s", arguments.file_path);
    }
    else
    {
        LOG_WARN(&logger_main, "No file path provided");
    }

    LOG_TRACE(&logger_main, "Recepie: %s", arguments.recepie);
    LOG_INFO(&logger_main, "Building %s based off %s", arguments.recepie, arguments.file_path);

    free(arguments.file_path);
    free(arguments.recepie);

    if (logger_main.scope)
        free((char *)logger_main.scope);
    if (logger_main.name)
        free((char *)logger_main.name);
    if (logger_main.date_format)
        free((char *)logger_main.date_format);
    if (logger_main.file)
        free((char *)logger_main.file);
    if (logger_main.function)
        free((char *)logger_main.function);

    return 0;
}
