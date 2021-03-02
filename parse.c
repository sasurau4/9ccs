#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "9ccs.h"

void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

/**
 * Keywords
 * */
char *KW_RETURN = "return";
char *KW_IF = "if";
char * KW_ELSE = "else";

/**
 * Tokenizer
 * */
bool consume(char *op) {
    switch (token->kind) {
        case TK_IDENT:
        case TK_NUM:
        case TK_EOF: {
            return false;
        }
        default: {
            // Do nothing
        }
    }

    if (strlen(op) != token->len || 
        memcmp(token->str, op, token->len)) {
        return false;
    }
    token = token->next;
    return true;
}

Token *consume_ident() {
    if (token->kind != TK_IDENT) {
        return 0;
    }
    Token *ident_token = token;
    token = token->next;
    return ident_token;
}

void expect(char *op) {
    if (token->kind != TK_RESERVED || 
        strlen(op) != token->len || 
        memcmp(token->str, op, token->len)) {
        printf("expect, '%s'.", op);
        error_at(token->str, op);
    }
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM) {
        error_at(token->str, "Not a number");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

bool starts_with(const char *pre, const char *str) {
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

bool is_alnum(char c) {
    return ('a' <= c && c <= 'z') || 
            ('A' <= c && c <= 'Z') || 
            ('0' <= c && c <= '9') || 
            (c == '_');
}

bool is_reserved_keyword(const char *str, const char *keyword) {
    size_t lenkw = strlen(keyword);
    return strncmp(str, keyword, lenkw) == 0 && !is_alnum(str[lenkw]);
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

LVar *find_lvar(Token *tok) {
    for (LVar *var = locals; var; var = var->next) {
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len)) {
            return var;
        }
    }
    return NULL;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (starts_with("==", p) || 
            starts_with("!=", p) || 
            starts_with(">=", p) ||
            starts_with("<=", p)) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p = p+2;
            continue;
        }

        switch (*p) {
            case '+': 
            case '-': 
            case '*': 
            case '/': 
            case '(': 
            case ')': 
            case '<': 
            case '>': 
            case '=': 
            case ';': {
                cur = new_token(TK_RESERVED, cur, p++, 1);
                continue;
            }
            default: {
                // Do nothing
            }
        }

        if (is_reserved_keyword(p, KW_RETURN)) {
            int keylen = strlen(KW_RETURN);
            cur = new_token(TK_RETURN, cur, p, keylen);
            p += keylen;
            continue;
        }

        if (is_reserved_keyword(p, KW_IF)) {
            int keylen = strlen(KW_IF);
            cur = new_token(TK_IF, cur, p, keylen);
            p += keylen;
            continue;
        }

        if (is_reserved_keyword(p, KW_ELSE)) {
            int keylen = strlen(KW_ELSE);
            cur = new_token(TK_ELSE, cur, p, keylen);
            p += keylen;
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        if (is_alnum(*p)) {
            int i = 1;
            while(is_alnum(*(p + i))) {
                i += 1;
                continue;
            }
            cur = new_token(TK_IDENT, cur, p, i);
            p += i;
            continue;
        }


        error_at(token->str, "Can't tokenize");
    }

    new_token(TK_EOF, cur, p, 1);
    return head.next;
}

/**
 * Parser
 * */

/**
 * function implementation
 * */
Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_ident();
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar *lvar = find_lvar(tok);
        if (lvar) {
            node->offset = lvar->offset;
        } else {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals ? locals : NULL;
            lvar->name = tok->str;
            lvar->len = tok->len;
            lvar->offset = locals ? locals->offset + 8 : 8;
            node->offset = lvar->offset;
            locals = lvar;
        }
        return node;
    }

    return new_node_num(expect_number());
}

Node *mul() {
    Node *node = unary();

    for(;;) {
        if (consume("*")) {
            node = new_node(ND_MUL, node, unary());
        } else if (consume("/")) {
            node = new_node(ND_DIV, node, unary());
        } else {
            return node;
        }
    }
}

Node *unary() {
    if (consume("+")) {
        return primary();
    }
    if (consume("-")) {
        return new_node(ND_SUB, new_node_num(0), primary());
    }
    return primary();

}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+")) {
            node = new_node(ND_ADD, node, mul());
        } else if(consume("-")) {
            node = new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<")) {
            node = new_node(ND_LT, node, add());
        } else if (consume(">")) {
            node = new_node(ND_LT, add(), node);
        } else if (consume("<=")) {
            node = new_node(ND_LE, node, add());
        } else if (consume(">=")) {
            node = new_node(ND_LE, add(), node);
        } else {
            return node;
        }
    }
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("==")) {
            node = new_node(ND_EQ, node, relational());
        } else if (consume("!=")) {
            node = new_node(ND_NOT_EQ, node, relational());
        } else {
            return node;
        }
    }
}

Node *assign() {
    Node *node = equality();

    if(consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

Node *expr() {
    return assign();
}

Node *stmt() {
    Node *node;

    if (consume(KW_RETURN)) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_RETURN;
        node->lhs = expr();
        expect(";");
    } else if (consume(KW_IF)) {
        expect("(");
        node = calloc(1, sizeof(Node));
        node->kind = ND_IF;
        node->cond = expr();
        expect(")");
        node->then = stmt();
        if (consume(KW_ELSE)) {
            node->els = stmt();
        }
    } else {
        node = expr();
        expect(";");
    }

    return node;
}

void program() {
    int i = 0;
    while (!at_eof()) {
        code[i++] = stmt();
    }
    code[i] = NULL;
}