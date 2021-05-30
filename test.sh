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