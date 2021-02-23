#include <stdarg.h>
#include <stdio.h>
#include "9ccs.h"

/**
 * Globals
 * */
char *user_input;
Token *token;
Node *code[100];
LVar *locals;

int count_locals_space() {
    int i = 0;
    if (!locals) {
        return i;
    }
    for (LVar *var = locals; var; var = var->next) {
        i += 1;
    }
    // A variable space is fixed of 8
    return i * 8;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        return 1;
    }

    user_input = argv[1];
    token = tokenize(user_input);
    program();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main: \n");

    // Prologue
    // Acquire space for variables
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, %d\n", count_locals_space());

    for (int i = 0; code[i]; i++) {
        gen(code[i]);

        // A value left as the result of statement at stack
        // So pop it to prevent overflow the stack
        printf("    pop rax\n");
    }

    // Epilogue
    // The result of last statement left on RAX, we return it
    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");
    return 0;
}