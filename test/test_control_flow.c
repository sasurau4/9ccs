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
int case_while_1() {
    int a;
    a = 1;
    while (a<4) a = a+1;
    return a;
}
int case_for_1() {
    int a;
    a=10;
    // TODD This is not C compatible, function return 0 if that hasn't return keyword.
    for (int b=0;b<3; b=b+1) a=a+1;a;
}
int case_for_2() {
    int a;a=10;
    for (int b=0;b<3; b=b+1) if(a==11) return a; else a=a+1;a;
}
int case_for_3() {
    int a;int b;a=10;b=0;
    for (;b<3; b=b+1) a=a+1;a;
}
int case_for_4() { 
    int a;a=10;
    for (int b=0;;b=b+1) a=a+1;a; 
}
int case_for_5() { 
    // TODD This is not C compatible, thee last b is undefined variable.
    for (int b=0;b<3;) b=b+1;b; 
}
int case_for_6() { 
    int a;a=1;
    for (;;) return a;
}
int case_for_7() { 
    int a;a=11;
    for (;;) return 12;
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
    test("while_1", 4, case_while_1());
    test("for_1", 13, case_for_1());
    test("for_2", 11, case_for_2());
    test("for_3", 13, case_for_3());
    test("for_4", 10, case_for_4());
    test("for_5", 3, case_for_5());
    test("for_6", 1, case_for_6());
    test("for_7", 12, case_for_7());

    return 0;
}