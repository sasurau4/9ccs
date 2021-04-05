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

// TODO: refactor to Map
char *gen_arg_reg_name(int i) {
    switch(i) {
        case 0: {
            return "rdi";
        }
        case 1: {
            return "rsi";
        }
        case 2: {
            return "rdx";
        }
        case 3: {
            return "rcx";
        }
        case 4: {
            return "r8";
        }
        case 5: {
            return "r9";
        }
        default: {
            error("Argument count exceed 6 but %i\n", i);
        }
    }
}

void gen_lval(Node *node) {
    if (node->kind != ND_LVAR) {
        error("Left value of assignment is not variable");
    }

    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");

}

void gen_func(Function *func) {
    // Prologue
    printf(".globl %s\n", func->name);
    printf("%s: \n", func->name);

    // Acquire space for variables
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    // A variable space is fixed of 8
    printf("    sub rsp, %d\n", func->lvars->len * 8);

    // Gen params
    for(int i = 0; i < func->node->params->len; i++) {
        Node *node = func->node->params->data[i];
        printf("    mov rax, rbp\n");
        printf("    sub rax, %d\n", node->offset);
        printf("    mov [rax], %s\n", gen_arg_reg_name(i));
    }

    gen(func->node->body);

    // Epilogue
    // The result of last statement left on RAX, we return it
    printf("    # Epilogue for func\n");
    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");
}

void gen(Node *node) {
    printf("    # debug: node->kind: %i\n", node->kind);
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
            // Is it correct to handle special for ND_CALL?
            if (node->lhs->kind != ND_CALL) {
                // All operations except function call left calculated value at stack top.
                // function call already left return value at RAX, so not pop stack to RAX.
                printf("    pop rax\n");
            }
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
            }
            return;
        }
        case ND_CALL: {
            for (int i = 0; i < node->args->len; i++) {
                gen(node->args->data[i]);
                char *register_name = gen_arg_reg_name(i);
                printf("    pop rax\n");
                printf("    mov %s, rax\n", register_name);
            }
            printf("    call %s\n", node->name);
            printf("    push rax\n");
            return;
        }
        case ND_ADDR: {
            gen_lval(node->lhs);
            return;
        }
        case ND_DEREF: {
            gen(node->lhs);
            printf("    pop rax\n");
            printf("    mov rax, [rax]\n");
            printf("    push rax\n");
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