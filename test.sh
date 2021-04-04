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
## assert 1 "
## fibonacci(x) {
##     if (x < 2) return x;
##     return fibonacci(x - 2);
## }
## main() { return fibonacci(2); }"
## exit 0
assert 0 'main() { return 0; }'
assert 42 "main() { return 42; }"
assert 25 "main() { return 5+20; }"
assert 21 "main() { return 5+20-4; }"
assert 41 "main() { return 12 + 34 - 5 ; }"
assert 47 'main() { return 5+6*7; }'
assert 15 'main() { return 5*(9-6); }'
assert 4 'main() { return (3+5)/2; }'
assert 5 'main() { return +5; }'
assert 10 'main() { return -10+20; }'
assert 1 'main() { return 2 == 2; }'
assert 0 'main() { return 12 == 1; }'
assert 1 'main() { return 100 != 1; }'
assert 0 'main() { return 100 != 2 * 50; }'
assert 1 'main() { return 1 < 2; }'
assert 0 'main() { return 1 < 1; }'
assert 0 'main() { return 1 > 1; }'
assert 0 'main() { return 2 <= 1; }'
assert 1 'main() { return 1 <= 1; }'
assert 1 'main() { return 1 >= 1; }'
assert 0 'main() { return 1 >= 5; }'
assert 0 'main() { return 4+6*7 < 46; }'
assert 1 'main() { return 4+6*7 <= 46; }'
assert 52 'main() { return a = 52; a; }'
assert 1 "main() { a = 1;
return a;
}"
assert 14 "main() { a = 3;
b = 5 * 6 - 8;
return a + b / 2; }"
assert 2 "main() { a = 1;
return a = a + 1; }"
assert 6 "main() { foo = 1;
bar = 2 + 3;
return foo + bar; }"
assert 2 "main() { return 2; }"
assert 2 "main() { return 2;
return 1; }"
assert 6 "main() { foo = 1;
bar = 2 + 3;
return foo + bar; }"
assert 2 'main() { if (1) return 2; }'
assert 10 'main() { if (0) return 2; else return 10; }'
assert 2 "main() { a = 1;
b = 1;
if (a - b) return a - b; else return a + b; }"
assert 4 "main() { a = 1;
while (a < 4) a = a + 1; return a; }"
assert 13 "main() { a = 10; for (b = 0; b < 3; b = b + 1) a = a + 1; a; }"
assert 11 "main() { a = 10; for (b = 0; b < 3; b = b + 1) if (a == 11) return a; else a = a + 1; a; }"
assert 13 "main() { a = 10; b = 0; for (; b < 3; b = b + 1) a = a + 1; a; }"
assert 10 "main() { a = 10; for (b = 0;; b = b + 1) a = a + 1; a; }"
assert 3 "main() { for (b = 0; b < 3;) b = b + 1; b; }"
assert 1 "main() { a = 1; for (;;) return a; }"
assert 1 "main() { a = 1; for (;;) return 1; }"
assert 1 "main() { { return 1; } }"
assert 10 "main() { if (1) {
    a = 10;
    b = 2;
    return a;
} else {
    a = 1;
    b = 3;
    return b;
} }"
assert 1 "main() { shout(); return 1; }";
assert 7 "main() { return sum(3, 4); }";
assert 2 "
main() {
    return 2;
}
"
assert 3 "
buzz() {
    return 3;
}
main() {
    return buzz();
}
"
assert 4 "
buzz(x) {
    return x;
}

main() {
    return buzz(4);
}
"
assert 5 "
double(x) {
    return x;
}
triple(x) {
    return x;
}

main() {
    return double(2) + triple(3);
}
"
assert 3 "
buzz(x) {
    return x;
}

main() {
    return buzz(4 - 1);
}
"
assert 4 "
buzz(x, y) {
    return x + y;
}

main() {
    return buzz(1, 3);
}
"
assert 19 "
buzz(x, y, z) {
    return x * 3 + y * 2 + z;
}

main() {
    return buzz(1, 3, 10);
}
"

echo OK