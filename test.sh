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
assert 0 'int main() { return 0; }'
assert 42 "int main() { return 42; }"
assert 25 "int main() { return 5+20; }"
assert 21 "int main() { return 5+20-4; }"
assert 41 "int main() { return 12 + 34 - 5 ; }"
assert 47 'int main() { return 5+6*7; }'
assert 15 'int main() { return 5*(9-6); }'
assert 4 'int main() { return (3+5)/2; }'
assert 5 'int main() { return +5; }'
assert 10 'int main() { return -10+20; }'
assert 1 'int main() { return 2 == 2; }'
assert 0 'int main() { return 12 == 1; }'
assert 1 'int main() { return 100 != 1; }'
assert 0 'int main() { return 100 != 2 * 50; }'
assert 1 'int main() { return 1 < 2; }'
assert 0 'int main() { return 1 < 1; }'
assert 0 'int main() { return 1 > 1; }'
assert 0 'int main() { return 2 <= 1; }'
assert 1 'int main() { return 1 <= 1; }'
assert 1 'int main() { return 1 >= 1; }'
assert 0 'int main() { return 1 >= 5; }'
assert 0 'int main() { return 4+6*7 < 46; }'
assert 1 'int main() { return 4+6*7 <= 46; }'
assert 52 'int main() { int a; return a = 52; a; }'
assert 1 "int main() { int a; a = 1;
return a;
}"
assert 14 "int main() { int a; a = 3;
int b;
b = 5 * 6 - 8;
return a + b / 2; }"
assert 2 "int main() { int a; a = 1;
return a = a + 1; }"
assert 10 "int main() { int a = 10; return a; }"
assert 6 "int main() { int foo; foo = 1;
int bar;
bar = 2 + 3;
return foo + bar; }"
assert 2 "int main() { return 2; }"
assert 2 "int main() { return 2;
return 1; }"
assert 2 'int main() { if (1) return 2; }'
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
    z = &y + 8;
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
    allocp(&p, 30, 31);
    q = *p + 3;
    return q;
}"
# assert 6 "
# int main() {
#     int *p = alloc1(3, 6);
#     return *(p + 1);
# }"
echo OK