#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./9ccs "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 '0;'
assert 42 "42;"
assert 25 "5+20;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
assert 5 '+5;'
assert 10 '-10+20;'
assert 1 '2 == 2;'
assert 0 '12 == 1;'
assert 1 '100 != 1;'
assert 0 '100 != 2 * 50;'
assert 1 '1 < 2;'
assert 0 '1 < 1;'
assert 0 '1 > 1;'
assert 0 '2 <= 1;'
assert 1 '1 <= 1;'
assert 1 '1 >= 1;'
assert 0 '1 >= 5;'
assert 0 '4+6*7 < 46;'
assert 1 '4+6*7 <= 46;'
assert 52 'a = 52;a;'
assert 1 "a = 1;
a;
"
assert 14 "a = 3;
b = 5 * 6 - 8;
a + b / 2;"
assert 2 "a = 1;
a = a + 1;"
assert 6 "foo = 1;
bar = 2 + 3;
foo + bar;"
assert 2 "return 2;"
assert 2 "return 2;
return 1;"
assert 6 "foo = 1;
bar = 2 + 3;
return foo + bar;"
assert 2 'if (1) 2;'
assert 10 'if (0) 2; else 10;'
assert 2 "a = 1;
b = 1;
if (a - b) a - b; else a + b;"
assert 4 "a = 1;
while (a < 4) a = a + 1;"
assert 13 "a = 10; for (b = 0; b < 3; b = b + 1) a = a + 1; a;"
assert 11 "a = 10; for (b = 0; b < 3; b = b + 1) if (a == 11) return a; else a = a + 1; a;"
assert 13 "a = 10; b = 0; for (; b < 3; b = b + 1) a = a + 1; a;"
assert 10 "a = 10; for (b = 0;; b = b + 1) a = a + 1; a;"
assert 3 "for (b = 0; b < 3;) b = b + 1; b;"
assert 1 "a = 1; for (;;) return a;"
assert 1 "a = 1; for (;;) return 1;"

echo OK