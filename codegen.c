#include "9ccs.h"

// Use for label name for control flow
int total_control_flow_count = 0;

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
    // For one line stmts
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
        case ND_RETURN: {
            gen(node->lhs);
            printf("    pop rax\n");
            printf("    mov rsp, rbp\n");
            printf("    pop rbp\n");
            printf("    ret\n");
            return;
        }
        case ND_IF: {
            int cfcount = total_control_flow_count;
            total_control_flow_count += 1;
            gen(node->cond);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je  .Lelse%03d\n", cfcount);
            gen(node->then);
            printf("    jmp .Lend%03d\n", cfcount);
            printf(".Lelse%03d:\n", cfcount);
            if (node->els) {
                gen(node->els);
            }
            printf(".Lend%03d:\n", cfcount);
            return;
        }
        case ND_WHILE: {
            int cfcount = total_control_flow_count;
            total_control_flow_count += 1;
            printf(".Lbegin%03d:\n", cfcount);
            gen(node->cond);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je  .Lend%03d\n", cfcount);
            gen(node->body);
            printf("    jmp .Lbegin%03d\n", cfcount);
            printf(".Lend%03d:\n", cfcount);
            return;
        }
        case ND_FOR: {
            int cfcount = total_control_flow_count;
            total_control_flow_count += 1;
            if (node->init) {
                gen(node->init);
            }
            printf(".Lbegin%03d:\n", cfcount);
            if (node->cond) {
                gen(node->cond);
            } else {
                printf("    push 0\n");
            } 
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je  .Lend%03d\n", cfcount);
            gen(node->body);
            if (node->inc) {
                gen(node->inc);
            }
            printf("    jmp .Lbegin%03d\n", cfcount);
            printf(".Lend%03d:\n", cfcount);
            return;
        }
        case ND_BLOCK: {
            for (int i = 0; i < node->stmts->len; i++) {
                gen(node->stmts->data[i]);
                printf("    pop rax\n");
            }
            return;
        }
        case ND_CALL: {
            for (int i = 0; i < node->args->len; i++) {
                gen(node->args->data[i]);
                char *register_name;
                switch(i) {
                    case 0: {
                        register_name = "rdi";
                        break;
                    }
                    case 1: {
                        register_name = "rsi";
                        break;
                    }
                    case 2: {
                        register_name = "rdx";
                        break;
                    }
                    case 3: {
                        register_name = "rcx";
                        break;
                    }
                    case 4: {
                        register_name = "r8";
                        break;
                    }
                    case 5: {
                        register_name = "r9";
                        break;
                    }
                }
                printf("    pop rax\n");
                printf("    mov %s, rax\n", register_name);
            }
            printf("    call %s\n", node->name);
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