#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

const char *argp_program_version = "spbuild 1.0";
const char *argp_program_bug_address = "sphynx@shittydev.com";

static char doc[] = "A modern build system, made to be easy";
static char args_doc[] = "[TARGET]";

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output", 0},
    {"file", 'f', "FILE", 0, "Specify the file path", 0},
    {0}
};

struct arguments {
    int verbose;
    char *file_path;
    char *target;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'v':
            arguments->verbose = 1;
            break;
        case 'f':
            arguments->file_path = arg;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num == 0) {
                arguments->target = arg;
            } else {
                argp_usage(state);
            }
            break;
        case ARGP_KEY_END:
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
    NULL,   // children
    NULL,   // group
    0       // flags
};

int main(int argc, char **argv) {
    struct arguments arguments;

    arguments.verbose = 0;
    arguments.file_path = NULL;
    arguments.target = NULL;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.file_path) {
        printf("File path: %s\n", arguments.file_path);
    } else {
        printf("No file path provided\n");
    }
    
    if (arguments.target) {
        printf("Target: %s\n", arguments.target);
    } else {
        printf("No target specified\n");
    }

    printf("Verbose mode: %s\n", arguments.verbose ? "enabled" : "disabled");

    return 0;
}
