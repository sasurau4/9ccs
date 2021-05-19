#include <stdarg.h>
#include <stdio.h>
#include "9ccs.h"

/**
 * Globals
 * */
char *user_input;
Token *token;
Program *program;
Map *lvars;
Map *gvars;
Map *funcs;
Vector *found_strs;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        return 1;
    }

    user_input = argv[1];
    token = tokenize(user_input);
    found_strs = new_vec();
    program = parse();

    gen_program(program);
    return 0;
}