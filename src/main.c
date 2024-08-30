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
        arguments->verbose = 1;
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
    logger_ctx *logger_main = logger_init("Build", LOG_LEVEL_INFO);

    arguments.file_path = NULL;
    arguments.recepie = NULL;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.verbose)
    {
        logger_main->min_level = LOG_LEVEL_DEBUG;
    }

    if (arguments.file_path)
    {
        log_debug(logger_main, "File path: %s", arguments.file_path);
        free(arguments.file_path);
    }
    else
    {
        log_warning(logger_main, "No file path provided");
    }

    log_debug(logger_main, "Recepie: %s", arguments.recepie);
    free(arguments.recepie);

    logger_free(logger_main);

    return 0;
}
