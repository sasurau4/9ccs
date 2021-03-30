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
};

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQ,
    ND_NOT_EQ,
    ND_LT,
    ND_LE,
    ND_NUM,
    ND_ASSIGN,
    ND_LVAR,
    ND_RETURN,
    ND_IF,
    ND_WHILE,
    ND_FOR,
    ND_BLOCK,
    ND_CALL,
} NodeKind;

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

    Vector *stmts; // Compound statement
    Vector *args; // Function call
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
    LVar *next; // Next var or NULL
    char *name; // The name of var
    int len; // The length of var name
    int offset; // Offset from RBP
};

/**
 * parser
 * */
Token *tokenize(char *p);
void program();
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

/**
 * Globals
 * */
extern Token *token;
extern char *user_input;
extern Node *code[100];
extern LVar *locals;