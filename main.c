#include <stdarg.h>
#include <stdio.h>
#include "9ccs.h"

/**
 * Globals
 * */
char *user_input;
Token *token;
Node *code[100];
Program *program;
Vector *lvars;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        return 1;
    }

    user_input = argv[1];
    token = tokenize(user_input);
    program = parse();

    printf(".intel_syntax noprefix\n");
    // TODO: move to codegen
    // printf(".globl main\n");
    // printf("main: \n");

    for (int i = 0; i < program->funcs->len; i++) {
        gen_func(program->funcs->data[i]);
    }
    // Prologue
    // Acquire space for variables
    // printf("    push rbp\n");
    // printf("    mov rbp, rsp\n");
    // printf("    sub rsp, %d\n", count_locals_space());

    // for (int i = 0; code[i]; i++) {
    //     gen(code[i]);

    //     // A value left as the result of statement at stack
    //     // So pop it to prevent overflow the stack
    //     printf("    pop rax\n");
    // }

    // // Epilogue
    // // The result of last statement left on RAX, we return it
    // printf("    mov rsp, rbp\n");
    // printf("    pop rbp\n");
    // printf("    ret\n");
    return 0;
}