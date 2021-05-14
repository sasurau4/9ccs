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
Map *funcs;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        return 1;
    }

    user_input = argv[1];
    token = tokenize(user_input);
    program = parse();

    printf(".intel_syntax noprefix\n");

    for (int i = 0; i < program->funcs->keys->len; i++) {
        gen_func(program->funcs->vals->data[i]);
    }
    return 0;
}