#include "9ccs.h"

// Use for label name for control flow
int total_control_flow_count = 0;

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
    if (node->kind == ND_LVAR) {
        assert(node->type);
        if (node->type->is_static) {
            printf("# uni_num: %03d\n", node->type->unique_num);
            char static_lvar_label[100];
            snprintf(static_lvar_label, sizeof(static_lvar_label),"%s.%03d", node->name, node->type->unique_num);
            printf("    lea rax, %s[rip]\n", static_lvar_label);
            printf("    push rax\n");
            return;
        } else {
            printf("    mov rax, rbp\n");
            printf("    sub rax, %d\n", node->offset);
            printf("    push rax\n");
            return;
        }
    } else if (node->kind == ND_DEREF) {
        gen(node->lhs);
        return;
    } else if (node->kind == ND_GVAR) {
        // Note: It's OK like "QWORD PTR hoge[rip]"
        printf("    lea rax, %s[rip]\n", node->name);
        printf("    push rax\n");
        return;
    }
    error("Left value of assignment is not variable, actual: %i\n", node->kind);
}

int calc_alignment(Type *ty) {
    if (ty->ty == INT) {
        return 4;
    } else if (ty->ty == CHAR) {
        return 1;
    } else if (ty->ty == PTR) {
        return 8;
    }
    assert(ty->ty == ARRAY);
    int need_byte = calc_need_byte(ty);
    if (need_byte > 31) {
        return 32;
    } else if (need_byte > 15) {
        return 16;
    } else if (need_byte > 7) {
        return 8;
    }
    if (ty->ptr_to->ty == INT) {
        return 4;
    } else if (ty->ptr_to->ty == CHAR) {
        return 1;
    } 
    error("Allignment can't detemined ty: %d\n", ty->ty);
}
void gen_func(Function *func) {
    // Acquire space for static local variables
    for (int i = 0; i < func->lvars->keys->len; i++) {
        Var *var = func->lvars->vals->data[i];
        if (var->type->is_static) {
            char *static_lvar_label;
            sprintf(static_lvar_label, "%s.%03d", var->name, var->type->unique_num);
            int need_byte = calc_need_byte(var->type);
            printf("    .local %s\n", static_lvar_label);
            printf("    .comm %s,%d,%d\n", static_lvar_label, need_byte, calc_alignment(var->type));
        }
    }
    // Function def
    printf(".globl %s\n", func->name);
    printf("%s: \n", func->name);
    // Prologue
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");

    // Acquire space for local variables
    if (func->lvars->keys->len > 0) {
        Var *last_lvar = vec_last(func->lvars->vals);
        printf("    sub rsp, %d\n", last_lvar->offset);
    }

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
        case ND_GVAR:
        case ND_LVAR: {
            gen_lval(node);
            if (node->type->ty == ARRAY) {
                return;
            }
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
            for (int i = node->args->len - 1; i > -1; i--) {
                gen(node->args->data[i]);
                char *register_name = gen_arg_reg_name(i);
                printf("    pop rax\n");
                printf("    mov %s, rax\n", register_name);
            }
            printf("    mov al, 0\n");
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
            if (node->lhs->type->ty == ARRAY && node->lhs->type->ptr_to->ty == CHAR) {
                printf("    movzx eax, BYTE PTR [rax]\n");
                printf("    movsx eax, al\n");
            } else {
                printf("    mov rax, [rax]\n");
            }
            printf("    push rax\n");
            return;
        }
        case ND_VARDEF: {
            if (node->init) {
                gen(node->init);
            }
            return;
        }
        case ND_STR: {
            printf("    lea rax, [rip+.LC%d]\n", node->str_index);
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
            if (node->lhs->type->ty == PTR || node->lhs->type->ty == ARRAY) {
                printf("    push rax\n");
                printf("    mov rax, %d\n", size_of(node->lhs->type->ptr_to));
                printf("    imul rdi, rax\n");
                printf("    pop rax\n");
            } 
            printf("    add rax, rdi\n");
            break;
        }
        case ND_SUB: {
            if (node->lhs->type->ty == PTR || node->lhs->type->ty == ARRAY) {
                printf("    push rax\n");
                printf("    mov rax, %d\n", size_of(node->lhs->type->ptr_to));
                printf("    imul rdi, rax\n");
                printf("    pop rax\n");
            } 
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

void gen_gvardef(Var *gvar) {
    printf("%s:\n", gvar->name);
    printf("    .zero %d\n", calc_need_byte(gvar->type));
}

void gen_program(Program *program) {
    printf(".intel_syntax noprefix\n");

    for (int i = 0; i < found_strs->len; i++) {
        printf(".LC%d:\n", i);
        printf("    .string \"%s\"\n", (char *)found_strs->data[i]);
    }

    printf(".bss\n");
    for (int i = 0; i < program->gvars->keys->len; i++) {
        gen_gvardef(program->gvars->vals->data[i]);
    }

    printf(".text\n");
    for (int i = 0; i < program->funcs->keys->len; i++) {
        gen_func(program->funcs->vals->data[i]);
    }
}