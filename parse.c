#include "9ccs.h"

static Type int_ty = {INT, NULL, 1};

void error_at(Token *token, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int ln = token->ln;
    fprintf(stderr, "Error at %d:%d\n", ln, token->col);
    fprintf(stderr, "%s", token->str);
    while(token->next && token->next->ln == ln) {
        fprintf(stderr, " %s", token->next->str);
        token = token->next;
    }
    fprintf(stderr, "\n");
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
char *KW_ELSE = "else";
char *KW_WHILE = "while"; 
char *KW_FOR = "for";
char *KW_INT = "int";
char *KW_SIZEOF = "sizeof";

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

bool check_token(Token *t, char *op) {
    switch (t->kind) {
        case TK_IDENT:
        case TK_NUM:
        case TK_EOF: {
            return false;
        }
        default: {
            // Do nothing
        }
    }

    if (strlen(op) != t->len || 
        memcmp(t->str, op, t->len)) {
        return false;
    }
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
    if (token->kind == TK_INT && op == KW_INT) {
        token = token->next;
        return;
    }
    // TODO: refactor to kind base switch stmt
    if (token->kind != TK_RESERVED || 
        strlen(op) != token->len || 
        memcmp(token->str, op, token->len)) {
        char *err;
        sprintf(err, "Expect keyword: %s", op);
        error_at(token, err);
    }
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM) {
        error_at(token, "Not a number");
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

Token *new_token(TokenKind kind, Token *cur, char *str, int len, int ln, int col) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    char *tokstr = strndup(str, len);
    tok->str = tokstr;
    tok->len = len;
    tok->ln = ln;
    tok->col = col;
    cur->next = tok;
    return tok;
}

LVar *find_lvar(Token *tok) {
    return map_get(lvars, tok->str);
}

Function *find_func(Token *tok) {
    return map_get(funcs, tok->str);
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;
    int ln = 1;
    int col = 1;

    while (*p) {
        if (*p == '\n') {
            ln += 1;
            col = 0;
            p++;
            continue;
        }
        if (isspace(*p)) {
            col++;
            p++;
            continue;
        }

        if (starts_with("==", p) || 
            starts_with("!=", p) || 
            starts_with(">=", p) ||
            starts_with("<=", p)) {
            cur = new_token(TK_RESERVED, cur, p, 2, ln, col);
            p = p+2;
            col += 2;
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
            case ';': 
            case ',': 
            case '&': 
            case '[': 
            case ']': 
            case '{': 
            case '}': {
                cur = new_token(TK_RESERVED, cur, p++, 1, ln, col++);
                continue;
            }
            default: {
                // Do nothing
            }
        }

        if (is_reserved_keyword(p, KW_RETURN)) {
            int keylen = strlen(KW_RETURN);
            cur = new_token(TK_RETURN, cur, p, keylen, ln, col);
            p += keylen;
            col += keylen;
            continue;
        }

        if (is_reserved_keyword(p, KW_IF)) {
            int keylen = strlen(KW_IF);
            cur = new_token(TK_IF, cur, p, keylen, ln, col);
            p += keylen;
            col += keylen;
            continue;
        }

        if (is_reserved_keyword(p, KW_ELSE)) {
            int keylen = strlen(KW_ELSE);
            cur = new_token(TK_ELSE, cur, p, keylen, ln, col);
            p += keylen;
            col += keylen;
            continue;
        }

        if (is_reserved_keyword(p, KW_WHILE)) {
            int keylen = strlen(KW_WHILE);
            cur = new_token(TK_ELSE, cur, p, keylen, ln, col);
            p += keylen;
            col += keylen;
            continue;
        }

        if (is_reserved_keyword(p, KW_FOR)) {
            int keylen = strlen(KW_FOR);
            cur = new_token(TK_FOR, cur, p, keylen, ln, col);
            p += keylen;
            col += keylen;
            continue;
        }

        if (is_reserved_keyword(p, KW_INT)) {
            int keylen = strlen(KW_INT);
            cur = new_token(TK_INT, cur, p, keylen, ln, col);
            p += keylen;
            col += keylen;
            continue;
        }

        if (is_reserved_keyword(p, KW_SIZEOF)) {
            int keylen = strlen(KW_SIZEOF);
            cur = new_token(TK_SIZEOF, cur, p, keylen, ln, col);
            p += keylen;
            col += keylen;
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0, ln, col);
            cur->val = strtol(p, &p, 10);
            char valstr[8];
            sprintf(valstr, "%d", cur->val);
            col += strlen(valstr);
            continue;
        }

        if (is_alnum(*p)) {
            int i = 1;
            while(is_alnum(*(p + i))) {
                i += 1;
                continue;
            }
            cur = new_token(TK_IDENT, cur, p, i, ln, col);
            p += i;
            col += i;
            continue;
        }


        error_at(token, "Can't tokenize");
    }

    new_token(TK_EOF, cur, p, 1, ln, col);
    return head.next;
}

/**
 * Parser
 * */

void swap(Node **p, Node **q) {
    Node *r = *p;
    *p = *q;
    *q = r;
}

void make_node_lhs_ptr(Node *node) {
    if (node->rhs->type->ty == PTR || node->rhs->type->ty == ARRAY) {
        swap(&node->rhs, &node->lhs);
        assert(node->lhs->type->ty == PTR || node->lhs->type->ty == ARRAY);
    }
}

/**
 * function implementation
 * */
Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    node->type = lhs->type;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;

    node->type = &int_ty;
    return node;
}

Node *new_node_lvar(Token *tok) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;

    LVar *lvar = find_lvar(tok);
    if (lvar) {
        node->offset = lvar->offset;
        node->type = lvar->type;
    } else {
        error_at(tok, "Local variable not defined.");
    }
    return node;
}

Node *new_node_array_index_access(Node *lhs, Node *rhs) {
    Node *array_access_index = new_node(ND_ADD, lhs, rhs);
    Node *array_access = new_node(ND_DEREF, array_access_index, NULL);
    array_access->type = array_access->lhs->type->ptr_to;
    return array_access;
}

LVar *new_lvar(Token *tok, Type *type) {
    LVar *lvar = calloc(1, sizeof(LVar));
    lvar->name = tok->str;
    lvar->len = tok->len;
    int prev_offset = 0;
    if (lvars->keys->len > 0) {
        LVar *last_lvar = vec_last(lvars->vals);
        prev_offset = last_lvar->offset;
    }
    if (type->ty == ARRAY) {
        lvar->offset = prev_offset + size_of(type->ptr_to) * type->array_size;
    } else {
        lvar->offset = prev_offset + 8;
    }
    lvar->type = type;
    return lvar;
}

void add_new_lvar() {
    Type *type = &int_ty;
    while (consume("*")) {
        Type *ptr_typ = calloc(1, sizeof(Type));
        ptr_typ->ty = PTR;
        ptr_typ->ptr_to = type;
        type = ptr_typ;
    }
    if (token->kind != TK_IDENT) {
        error_at(token, "Expect ident.");
    }
    if (check_token(token->next, "[")) {
        Type *array_type = calloc(1, sizeof(Type));
        array_type->array_size = token->next->next->val;
        array_type->ty = ARRAY;
        array_type->ptr_to = type;
        type = array_type;
    }
    LVar *lvar = new_lvar(token, type);
    map_put(lvars, lvar->name, lvar);
    return;
}

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_ident();
    if (tok) {
        if (consume("(")) {
            Node *node = calloc(1, sizeof(Node));
            node->kind = ND_CALL;
            node->name = tok->str;
            node->args = new_vec();
            node->type = &int_ty;
            for(;;) {
                if (consume(")")) {
                    return node;
                } else if (consume(",")){
                    Node *arg = expr();
                    vec_push(node->args, arg);
                } else {
                    Node *arg = expr();
                    vec_push(node->args, arg);
                }
                if (node->args->len > 6) {
                    error_at(token, "Function args exceed 6");
                }
            }
        }

        return new_node_lvar(tok);
    }

    return new_node_num(expect_number());
}

Node *mul() {
    Node *node = unary();

    for(;;) {
        if (consume("*")) {
            node = new_node(ND_MUL, node, unary());
            node->type = node->lhs->type;
        } else if (consume("/")) {
            node = new_node(ND_DIV, node, unary());
            node->type = node->lhs->type;
        } else {
            return node;
        }
    }
}

Node *unary() {
    if (consume("&")) {
        Node *node = new_node(ND_ADDR, unary(), NULL);
        Type *type = calloc(1, sizeof(Type));
        type->ty = PTR;
        type->ptr_to = node->lhs->type;
        node->type = type;
        return node;
    } else if (consume("*")) {
        Node *node = new_node(ND_DEREF, unary(), NULL);
        node->type = node->lhs->type->ptr_to;
        return node;
    }

    if (consume(KW_SIZEOF)) {
        Node *node = unary();
        return new_node_num(size_of(node->type));
    }

    Node *pri = calloc(1, sizeof(Node));
    if (consume("+")) {
        pri = primary();
    } else if (consume("-")) {
        Node *node = new_node(ND_SUB, new_node_num(0), primary());
        pri = node;
    } else {
        pri = primary();
    } 

    if (!consume("[")) {
        return pri;
    }
    // Support array access syntax like "2[a]" and "a[2]"
    Node *array_index_access_node = calloc(1, sizeof(Node));
    Node *array_access_index_node = expr();
    array_index_access_node = new_node_array_index_access(pri, array_access_index_node);
    expect("]");
    return array_index_access_node;
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+")) {
            node = new_node(ND_ADD, node, mul());
            make_node_lhs_ptr(node);
            node->type = node->lhs->type;
        } else if(consume("-")) {
            node = new_node(ND_SUB, node, mul());
            // if (node->rhs->type->ty == PTR) {
            //     error_at(token, "rhs cannot be PTR");
            // }
            node->type = node->lhs->type;
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
    // Handle var definition
    if (consume(KW_INT)) {
        add_new_lvar();
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_VARDEF;
        // Handle definition has initializer
        if (check_token(token->next, "=")) {
            node->init = assign();
        } else {
            // If definition doesn't have initilizer, consume ident
            consume_ident();
            // TODO: Implement array def with initializer
            if (consume("[")) {
                expect_number();
                expect("]");
            }
        }
        return node;
    }

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
    } else if (consume(KW_WHILE)) {
        expect("(");
        node = calloc(1, sizeof(Node));
        node->kind = ND_WHILE;
        node->cond = expr();
        expect(")");
        node->body = stmt();
    } else if (consume(KW_FOR)) {
        expect("(");
        node = calloc(1, sizeof(Node));
        node->kind = ND_FOR;
        if (!consume(";")) {
            node->init = expr();
            expect(";");
        }
        if (!consume(";")) {
            node->cond = expr();
            expect(";");
        }
        if (!consume(")")) {
            node->inc = expr();
            expect(")");
        }
        node->body = stmt();
    } else if (consume("{")) {
        Vector *stmts = new_vec();
        node = calloc(1, sizeof(Node));
        node->kind = ND_BLOCK;
        while (!consume("}")) {
            vec_push(stmts, stmt());
        }
        node->stmts = stmts;
    } else {
        node = expr();
        expect(";");
    }

    return node;
}

Program *parse() {
    Program *program;
    program = calloc(1, sizeof(Program));
    funcs = new_map();

    while(!at_eof()) {
        Node *node;
        Vector *params;

        Function *func;
        func = calloc(1, sizeof(Function));

        // LVars in func
        lvars = new_map();

        node = calloc(1, sizeof(Node));
        params = new_vec();
        node->kind = ND_FUNC;

        expect(KW_INT);
        Token *tok = consume_ident();
        if(!tok) {
            error_at(token, "Top level expect function defs");
        }
        func->name = tok->str;
        node->name = tok->str;
        expect("(");
        while(!consume(")")) {
            expect(KW_INT);
            add_new_lvar();
            Node *param = primary();
            vec_push(params, param);
            consume(",");
        }
        node->body = stmt();
        node->params = params;
        func->lvars = lvars;
        func->node = node;
        map_put(funcs, func->name, func);
    }

    program->funcs = funcs;
    return program;
}