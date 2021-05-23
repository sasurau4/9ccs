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
int case_return_1() {
    return 3;
}
int case_if_else_1() {
    int a;int b;
    if (1) {a=10;b=2;return a;} else {a=1;b=3;return b;}
}
int case_func_call_1() {
    // From test_func
    shout();return 1;
}
int case_func_call_2() {
    // From test_func
    return sum(3,4);
}
int helper_case_func_def_1() {
    return 3;
}
int case_func_def_1() {
    return helper_case_func_def_1();
}
int helper_case_func_def_2(int x) {
    return x;
}
int case_func_def_2() {
    return helper_case_func_def_2(4);
}
int helper_case_func_def_3_double(int x) {
    return x*2;
}
int helper_case_func_def_3_triple(int x) {
    return x*3;
}
int case_func_def_3() {
    return helper_case_func_def_3_double(2) + helper_case_func_def_3_triple(3);
}
int case_func_def_4() {
    return helper_case_func_def_2(4-1);
}
int helper_case_func_def_5(int x, int y) {
    return x+y;
}
int case_func_def_5() {
    return helper_case_func_def_5(1, 3);
}
int helper_case_func_def_6(int x, int y, int z) {
    return x*3+y*2+z;
}
int case_func_def_6() {
    return helper_case_func_def_6(1,3,10);
}
int helper_fibonacci(int x) {
    if (x<2) return x;
    return helper_fibonacci(x-1)+helper_fibonacci(x-2);
}
int case_fibonacci() {
    return helper_fibonacci(10);
}
int case_addr_1() {
    int x;int y;
    x=3;y=&x;return *y;
}
int case_addr_2() {
    int x;int y;int z;
    x=3;y=5;z=&y+2;return *z;
}
int case_deref_1() {
    int x;int *y;
    y = &x;*y=3;return x;
}
int case_ptr_plus_1() {
    int *p;int q;
    // From test_func
    allocp2(&p,30,31);
    q=*p+3;
    return q;
}
int case_ptr_plus_2() {
    // From test_func
    int *p = alloc1(3,6);
    return *(p+1);
} 
int case_ptr_plus_3() {
    // From test_func
    int *p;allocp2(&p,30,31);
    int *q;q=p+1;
    return *q;
} 
int case_ptr_plus_4() {
    // From test_func
    int *p;allocp4(&p,1,2,4,8);
    int *q;q=p+3;
    return *q;
} 
int case_sizeof_1() {
    int x;return sizeof(x);
}
int case_sizeof_2() {
    int x;return sizeof x;
}
int case_sizeof_3() {
    int *x;return sizeof(x);
}
int case_sizeof_4() {
    int x;return sizeof(x+3);
}
int case_sizeof_5() {
    int *x;return sizeof(x+3);
}
int case_sizeof_6() {
    int *x;return sizeof(*x);
}
int case_sizeof_7() {
    return sizeof(1);
}
int case_sizeof_8() {
    return sizeof(sizeof(1));
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
    test("return_1", 3, case_return_1());
    test("if_else_1", 10, case_if_else_1());
    test("func_call_1", 1, case_func_call_1());
    test("func_call_2", 7, case_func_call_2());
    test("func_call_2", 7, case_func_call_2());
    test("func_def_1", 3, case_func_def_1());
    test("func_def_2", 4, case_func_def_2());
    test("func_def_3", 13, case_func_def_3());
    test("func_def_4", 3, case_func_def_4());
    test("func_def_5", 4, case_func_def_5());
    test("func_def_6", 19, case_func_def_6());
    test("fibonacci", 55, case_fibonacci());

    test("addr_1", 3, case_addr_1());
    test("addr_2", 3, case_addr_2());
    test("deref_1", 3, case_deref_1());
    test("ptr_plus_1", 33, case_ptr_plus_1());
    test("ptr_plus_2", 6, case_ptr_plus_2());
    test("ptr_plus_3", 31, case_ptr_plus_3());
    test("ptr_plus_4", 8, case_ptr_plus_4());
    test("sizeof_1", 4, case_sizeof_1());
    test("sizeof_2", 4, case_sizeof_2());
    test("sizeof_3", 8, case_sizeof_3());
    test("sizeof_4", 4, case_sizeof_4());
    test("sizeof_5", 8, case_sizeof_5());
    test("sizeof_6", 4, case_sizeof_6());
    test("sizeof_7", 4, case_sizeof_7());
    test("sizeof_8", 4, case_sizeof_8());

    return 0;
}