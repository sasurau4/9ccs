#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./9ccs "$input" > tmp.s
    cc -o tmp tmp.s test_func.o
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}
# exit

assert 0 ./test/test_expr.c
assert 0 ./test/test_control_flow.c
exit
assert 10 'int main() { if (0) return 2; else return 10; }'
assert 2 "int main() { int a; int b; a = 1;
b = 1;
if (a - b) return a - b; else return a + b; }"
assert 4 "int main() { int a; a = 1;
while (a < 4) a = a + 1; return a; }"
assert 13 "int main() { int a; a = 10; for (int b = 0; b < 3; b = b + 1) a = a + 1; a; }"
assert 11 "int main() { int a; a = 10; for (int b = 0; b < 3; b = b + 1) if (a == 11) return a; else a = a + 1; a; }"
assert 13 "int main() { int a; int b; a = 10; b = 0; for (; b < 3; b = b + 1) a = a + 1; a; }"
assert 10 "int main() { int a; a = 10; for (int b = 0;; b = b + 1) a = a + 1; a; }"
assert 3 "int main() { for (int b = 0; b < 3;) b = b + 1; b; }"
assert 1 "int main() { int a; a = 1; for (;;) return a; }"
assert 1 "int main() { int a; a = 1; for (;;) return 1; }"
assert 1 "int main() { { return 1; } }"
assert 10 "int main() { 
int a;
int b;
if (1) {
    a = 10;
    b = 2;
    return a;
} else {
    a = 1;
    b = 3;
    return b;
} }"
assert 1 "int main() { shout(); return 1; }";
assert 7 "int main() { return sum(3, 4); }";
assert 2 "
int main() {
    return 2;
}
"
assert 3 "
int buzz() {
    return 3;
}
int main() {
    return buzz();
}
"
assert 4 "
int buzz(int x) {
    return x;
}

int main() {
    return buzz(4);
}
"
assert 13 "
int double(int x) {
    return x * 2;
}
int triple(int x) {
    return x * 3;
}

int main() {
    return double(2) + triple(3);
}
"
assert 3 "
int buzz(int x) {
    return x;
}

int main() {
    return buzz(4 - 1);
}
"
assert 4 "
int buzz(int x, int y) {
    return x + y;
}

int main() {
    return buzz(1, 3);
}
"
assert 19 "
int buzz(int x, int y, int z) {
    return x * 3 + y * 2 + z;
}

int main() {
    return buzz(1, 3, 10);
}
"
assert 55 "
int fibonacci(int x) {
    if (x < 2) return x;
    return fibonacci(x - 1) + fibonacci(x - 2);
}
int main() { return fibonacci(10); }"
assert 3 "
int main() {
    int x;
    int y;
    x = 3;
    y = &x;
    return *y;
}"
assert 3 "
int main() {
    int x;
    int y;
    int z;
    x = 3;
    y = 5;
    z = &y + 2;
    return *z;
}
"
assert 3 "
int main() {
    int x;
    int *y;
    y = &x;
    *y = 3;
    return x;
}
"
assert 33 "
int main() {
    int *p;
    int q;
    allocp2(&p, 30, 31);
    q = *p + 3;
    return q;
}"
assert 6 "
int main() {
    int *p = alloc1(3, 6);
    return *(p + 1);
}"
assert 31 "
int main() {
    int *p;
    allocp2(&p, 30, 31);
    int *q;
    q = p + 1;
    return *q;
}"
assert 8 "
int main() {
    int *p;
    allocp4(&p, 1, 2, 4, 8);
    int *q;
    q = p + 3;
    return *q;
}"
assert 4 "
int main() {
    int x;
    return sizeof(x);
}"
assert 4 "
int main() {
    int x;
    return sizeof x;
}"
assert 8 "
int main() {
    int *x;
    return sizeof(x);
}"
assert 4 "
int main() {
    int x;
    return sizeof(x + 3);
}"
assert 8 "
int main() {
    int *x;
    return sizeof(x + 3);
}"
assert 4 "
int main() {
    int *x;
    return sizeof(*x);
}"
assert 4 "
int main() {
    return sizeof(1);
}"
assert 4 "
int main() {
    return sizeof(sizeof(1));
}"
assert 0 "
int main() {
    int a[2];
    return 0;
}"
assert 2 "
int main() {
    int a[1];
    *a = 2;
    return *a;
}
"
assert 3 "
int main() {
    int a[2];
    *a = 2;
    *(a + 1) = 3;
    return *(a + 1);
}
"
assert 5 "
int main() {
    int a[2];
    *a = 2;
    *(a + 1) = 3;
    int *p;
    p = a;
    return *p + *(p + 1);
}
"
assert 4 "
int main() {
    int a[3];
    a[0] = 2;
    a[1] = 3;
    a[2] = 4;
    return a[2];
}"
assert 4 "
int main() {
    int a[3];
    a[1 + 1] = 4;
    return a[2];
}"
assert 4 "
int main() {
    int a[3];
    2[a] = 4;
    return 2[a];
}
"
assert 4 "
int main() {
    int a[3];
    (1+1)[a] = 4;
    return (1+1)[a];
}
"
assert 4 "
int main() {
    int a[3];
    (1+1)[a] = 4;
    return (1+1)[a];
}
"
assert 32 "
int main() {
    int a[2];
    a[0] = 11;
    a[1] = 21;
    return a[0] + a[1];
}
"
assert 62 "
int main() {
    int a[3];
    a[0] = 11;
    a[1] = 21;
    a[2] = 30;
    return a[0] + a[1] + a[2];
}
"
assert 0 "
int *foo;
int bar[10];
int *bazz() {}
int foobar() {}
int main() { return 0; }
"
assert 6 "
int foo;
int bar() {
    foo = 14;
    return foo;
}
int main() {
    int foo = 6;
    return foo;
}
"
assert 14 "
int foo;
int bar() {
    foo = 14;
    return foo;
}
int main() {
    int foo = 6;
    return bar();
}
"
assert 13 "
int bar[10];
int main() { 
    bar[1] = 13;
    return bar[1];
}
"
assert 31 "
int foo;
int foobar() {
    return 11;
}
int main() {
    foo = 20;
    return foobar() + foo;
}
"
assert 10 "
char main() {
    char x[2];
    x[1] = 10;
    return x[1];
}
"
assert 7 "
char x[2];
int main() {
    x[0] = -12;
    x[1] = 14;
    int y;
    y = 5;
    return x[0] + x[1] + y;
}
"
assert 10 '
int main() {
    char *x = "abc";
    printf(x);
    return 10;
}
'
echo OK