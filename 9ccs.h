#define _GNU_SOURCE
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef enum {
    TK_RESERVED,
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_FOR,
    TK_INT,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
    // For error info
    int ln;
    int col;
};

typedef enum {
    ND_ADD, // 0
    ND_SUB, // 1
    ND_MUL, // 2
    ND_DIV, // 3
    ND_EQ, // 4
    ND_NOT_EQ, // 5
    ND_LT, // 6
    ND_LE, // 7
    ND_NUM, // 8
    ND_ASSIGN, // 9
    ND_LVAR, // 10
    ND_RETURN, // 11
    ND_IF, // 12
    ND_WHILE, // 13
    ND_FOR, // 14
    ND_BLOCK, // 15
    ND_CALL, // 16
    ND_FUNC, // 17
    ND_ADDR, // 18
    ND_DEREF, // 19
    ND_VARDEF, // 20 but not used
} NodeKind;

typedef struct Type Type;

struct Type {
    enum { INT, PTR } ty;
    struct Type *ptr_to;
};

typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vec(void);
void vec_push(Vector *v, void *elem);
void vec_pushi(Vector *v, int val);
void *vec_pop(Vector *v);
void *vec_last(Vector *v);
bool vec_contains(Vector *v, void *elem);
bool vec_union1(Vector *v, void *elem);

typedef struct {
    Vector *keys;
    Vector *vals;
} Map;

Map *new_map(void);
void map_put(Map *map, char *key, void *val);
void map_puti(Map *map, char *key, int val);
void *map_get(Map *map, char *key);
int map_geti(Map *map, char *key, int default_);
bool map_exists(Map *map, char *key);

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;

    char *name;
    Type *type; // LVar

    Vector *stmts; // Compound statement
    Vector *args; // Function call
    Vector *params; // Function defenition
    // "if" (cond) then "else" els
    // "while" (cond) body
    // "for" (init; cond; inc) body
    Node *cond; 
    Node *then; 
    Node *els;
    Node *body;
    Node *init;
    Node *inc;
    int val; // If kind is ND_NUM, this hold value
    int offset; // If kind is ND_LVAR, this hold offset from RBP
};

typedef struct LVar LVar;

struct LVar {
    char *name; // The name of var
    int len; // The length of var name
    int offset; // Offset from RBP
    Type *type;
};

typedef struct {
    char *name;
    Node *node;
    Vector *lvars;
} Function;

typedef struct {
    Vector *funcs;
} Program;

/**
 * parser
 * */
Token *tokenize(char *p);
Program *parse();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

/**
 * codegen
 * */
void gen(Node *node);
void gen_func(Function *func);

/**
 * Globals
 * */
extern Token *token;
extern char *user_input;
extern Vector *lvars;
extern Vector *funcs;