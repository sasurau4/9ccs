int case1() {
    int a;
    return a = 52;
    a;
}

int case2() {
    int a; a = 1; return a;
}
int case3() { 
    int a; a = 3;
    int b;
    b = 5 * 6 - 8;
    return a + b / 2; 
}
int case4() { 
    int a; 
    a = 1;
    return a = a + 1; 
}
int case5() { 
    int a = 10; 
    return a; 
}
int case6() { 
    int foo; foo = 1;
    int bar;
    bar = 2 + 3;
    return foo + bar; 
}
int case7() { return 2; }
int case8() { return 2; return 1; }
int case_if_1() {
    if (1) return 2;
}
int case_if_2() {
    if (0) return 2; else return 10;
}
int case_if_3() {
    int a;
    int b;
    a = 1;
    b = 1;
    if (a-b) return a - b;
    else return a + b;
}

int main() { 
    test("stmt_1", 52, case1());
    test("stmt_2", 1, case2());
    test("stmt_3", 14, case3());
    test("stmt_4", 2, case4());
    test("stmt_5", 10, case5());
    test("stmt_6", 6, case6());
    test("stmt_7", 2, case7());
    test("stmt_8", 2, case8());
    test("if_1", 2, case_if_1());
    test("if_2", 10, case_if_2());
    test("if_3", 2, case_if_3());

    return 0;
}