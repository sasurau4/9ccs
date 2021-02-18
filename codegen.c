#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "9ccs.h"

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void gen_lval(Node *node) {
    if (node->kind != ND_LVAR) {
        error("Left value of assignment is not variable");
    }

    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");

}
void gen(Node *node) {
    switch (node->kind) {
        case ND_NUM: {
            printf("    push %d\n", node->val);
            return;
        }
        case ND_LVAR: {
            gen_lval(node);
            printf("    pop rax\n");
            printf("    mov rax, [rax]\n");
            printf("    push rax\n");
            return;
        }
        case ND_ASSIGN: {
            gen_lval(node->lhs);
            gen(node->rhs);

            printf("    pop rdi\n");
            printf("    pop rax\n");
            printf("    mov [rax], rdi\n");
            printf("    push rdi\n");
            return;
        }
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch (node->kind) {
        case ND_ADD: {
            printf("    add rax, rdi\n");
            break;
        }
        case ND_SUB: {
            printf("    sub rax, rdi\n");
            break;
        }
        case ND_MUL: {
            printf("    imul rax, rdi\n");
            break;
        }
        case ND_DIV: {
            printf("    cqo\n");
            printf("    idiv rdi\n");
            break;
        }
        case ND_EQ: {
            printf("    cmp rax, rdi\n");
            printf("    sete al\n");
            printf("    movzb rax, al\n");
            break;
        }
        case ND_NOT_EQ: {
            printf("    cmp rax, rdi\n");
            printf("    setne al\n");
            printf("    movzb rax, al\n");
            break;
        }
        case ND_LT: {
            printf("    cmp rax, rdi\n");
            printf("    setl al\n");
            printf("    movzb rax, al\n");
            break;
        }
        case ND_LE: {
            printf("    cmp rax, rdi\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
            break;
        }
    }

    printf("    push rax\n");
}