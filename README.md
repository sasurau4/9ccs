## 9ccs

9ccs is 9cc-syakyou edition. Syakyou means copy with hands in Japanese.

See https://www.sigbus.info/compilerbook

### Points

#### Debugging with gdb


```
gcc -g3 9ccs.c
gdb a.out
```

ref: 478b796bc2854ce3d1e6737d2bdd7e18943254c3
remove tokenize function call and compile, then run result `SIGSEGV` with Address boundary error!
https://qiita.com/arene-calix/items/a08363db88f21c81d351

#### Increment and assign

ref: e45cec0fc2283111391bc78810b114d26e9912d6

This cause bug token->str has no multiple operator. Ex, '5==6' -> token->str is '6'

```c
            cur = new_token(TK_RESERVED, cur, p+=2, 2);
            cur = new_token(TK_RESERVED, cur, p++, 1);
```

```c
x=0;
y = x++;
->
x = 1, y = 0
```

```c
x=0;
y= x+=1
->
x=1, y=1
```

#### gcc withc -Wl option

Incorrect
```
gcc --static -Wl, -N -o test test.o
/usr/bin/ld: cannot find : No such file or directory
collect2: error: ld returned 1 exit status
```

correct
```
gcc --static -Wl,-N -o test test.o
```

#### Split source files cause undefined reference

Incorrect and emit `undefined reference to "user_input"`

```c
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        return 1;
    }

    user_input = argv[1];
    token = tokenize(user_input);
    Node *node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main: \n");

    gen(node);

    printf("    pop rax\n");
    printf("    ret\n");
    return 0;
}
```

Correct
```c
char *user_input;
Token *token;

int main(int argc, char **argv) {
    ...
}
```

## Implement return stmt

Encounter this crash.

```
./test.sh
return 2;
 ^ Not a number
```

This is caused by `consume` function take into consideration only for `TK_RESERVED`. Adding for `TK_RETURN` fix this issue.

## Implement if and else stmt

Following is work fine and return 2

```assembly
.intel_syntax noprefix
.globl main
main: 
    push 2
    push 3
    pop rax
    pop rax
    ret
```

SIGSEGV with address boundary error

```assembly
.intel_syntax noprefix
.globl main
main: 
    push 2
    push 3
    pop rax
    ret
```

We need to clear arranged registers (or stacks?)

## Implement for

We need to handle `a = 1; for (;;) return a;`. If cond is null, generate `push 0\n` for cmp always is true.

## Impl function call

Missing `#define _GNU_SOURCE` and occured SIGSEGV.

https://stackoverflow.com/questions/5582211/what-does-define-gnu-source-imply

## Impl function definition

### stack and queue

Input `main() { a = 52; a; }`

I implemented `gen(vec_pop(node->stmts));` for `ND_FUNC` at `codegen.c`

But this input AST is `[[{kind: ND_ASSIGN}], [{kind: ND_LVAR}]]`

pop op break the order of node. For my case, compiler generate `ND_LVAR` first and then generate for `ND_ASSIGN`. 

I fixed it with `gen(node->stmts->data[i])`.

### Difference with 9ccs and real C semantics

All statements automatically return each value in my own C with 9ccs until now. For example, `main() { 2; }` return `2`.

This cause problem with `foo() { 2; } main() { foo(); }`. I need this return `2`.

But this code emits following and the return is `0`.

```assembly
.intel_syntax noprefix
.globl buzz
buzz: 
    push rbp
    mov rbp, rsp
    sub rsp, 0 # prologue
    push 3
    pop rax
    mov rsp, rbp # epilogue
    pop rbp
    ret
.globl main
main: 
    push rbp
    mov rbp, rsp
    sub rsp, 0 # prologue
    call buzz # <- RAX is 3
    pop rax # <- CAUSE: 3 is popped and RAX is 0
    mov rsp, rbp # epilogue
    pop rbp
    ret # <- return is 0

```

The `pop rax` after `call buzz` is unnecessary. It seems bad to prevent `pop rax` when `ND_CALL` because the AST is same between `foo() { 2; }` and `main() {foo();}`.

In real C, `int foo() { 2; } void main() { printf("foo: %i", foo())}` emit `foo: 0`.

So, C needs `return` statement to return value. Without it, no value returned. For example, https://godbolt.org/z/nvbdEf8fM

From now on, my own C need to follow real C semantics.

## ND_RETURN assumption

```c
            if (node->lhs->kind != ND_CALL) {
                // All operations except function call left calculated value at stack top.
                // function call already left return value at RAX, so not pop stack to RAX.
                printf("    pop rax\n");
            }
```