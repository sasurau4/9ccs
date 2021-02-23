
typedef enum {
    TK_RESERVED,
    TK_RETURN,
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
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
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