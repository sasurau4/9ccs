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
    for (int b=0;b<3; b=b+1) a=a+1;return a;
}
int case_for_2() {
    int a;a=10;
    for (int b=0;b<3; b=b+1) if(a==11) return a; else a=a+1;a;
}
int case_for_3() {
    int a;int b;a=10;b=0;
    for (;b<3; b=b+1) a=a+1;return a;
}
int case_for_4() { 
    int a;a=10;
    for (int b=0;;b=b+1) a=a+1;return a; 
}
int case_for_5() { 
    // TODD This is not C compatible, thee last b is undefined variable.
    for (int b=0;b<3;) b=b+1;return b; 
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
int case_static_1() {
    static int a; a=10;return a;
}
int case_array_1() {
    int a[2];return 0;
}
int case_array_2() {
    static int a[1];*a=2;return *a;
}
int case_array_3() {
    static int a[2]; *a = 2; *(a + 1) = 3;
    return *(a + 1);
}
int case_array_4() {
    static int a[2];*a = 2;*(a + 1) = 3;
    int *p;p = a;
    return *p + *(p + 1);
}
int case_array_access_1() {
    static int a[3];a[0] = 2;a[1] = 3;a[2] = 4;
    return a[2];
}
int case_array_access_2() {
    static int a[3];a[1 + 1] = 4;
    return a[2];
}
int case_array_access_3() {
    static int a[3];2[a] = 4;return 2[a];
}
int case_array_access_4() {
    static int a[3];(1+1)[a] = 4;return (1+1)[a];
}
int case_array_access_5() {
    static int a[2];a[0] = 11;a[1] = 21;
    return a[0] + a[1];
}
int case_array_access_6() {
    static int a[3];a[0] = 11;a[1] = 21;a[2] = 30;
    return a[0] + a[1] + a[2];
}
// Test for parsing global var and func def
int *parse_ptr_gvar;
int parse_garr[10];
int *parse_ptr_func() {}
int parse_func() {}

int case_gvar_1_foo;
int case_gvar_1_helper() {
    case_gvar_1_foo = 14;
    return case_gvar_1_foo;
}
int case_gvar_1() {
    case_gvar_1_foo = 6;
    return case_gvar_1_foo;
}

int case_gvar_2_foo;
int case_gvar_2_helper() {
    case_gvar_2_foo = 14;
    return case_gvar_2_foo;
}
int case_gvar_2() {
    case_gvar_2_foo = 6;
    return case_gvar_2_helper();
}

int case_gvar_3_foo[10];
int case_gvar_3() {
    case_gvar_3_foo[1] = 13;
    return case_gvar_3_foo[1];
}

int case_gvar_4_foo;
int case_gvar_4_helper() {
    return 11;
}
int case_gvar_4() {
    case_gvar_4_foo = 20;
    return case_gvar_4_helper() + case_gvar_4_foo;
}

int case_char_array_1() {
    static char x[2];x[1] = 10;
    return x[1];
}

char case_char_array_2_x[2];
int case_char_array_2() {
    case_char_array_2_x[0] = -12;
    case_char_array_2_x[1] = 14;
    int y;y = 5;
    return case_char_array_2_x[0] + case_char_array_2_x[1] + y;
}
int case_str_1() {
    char *x = "I'm abc\n";
    printf(x);
    return 10;
}

int main() { 
    /**
     * This is Block comment
     * */
    // Arithmetic ope
    test("ari1", 0, 0);
    test("ari2", 42, 42);
    test("ari3", 2, 1 + 1);
    test("ari4", 25, 5 + 20);
    test("ari5", 21, 5 + 20 - 4);
    test("ari6", 41, 12 + 34 -5);
    test("ari7", 47, 5+6*7);
    test("ari8", 15, 5*(9-6));
    test("ari9", 4, (3+5)/2);
    test("ari10", 5, +5);
    test("ari10", 10, -10+20);

    // Test relational operators
    test("relational1", 1, 2 == 2);
    test("relational2", 0, 12 == 2);
    test("relational3", 1, 100 != 1);
    test("relational4", 0, 100 != 2 * 50);
    test("relational5", 1, 1 < 2);
    test("relational6", 0, 1 < 1);
    test("relational7", 0, 1 > 1);
    test("relational8", 0, 2 <= 1);
    test("relational9", 1, 1 <= 1);
    test("relational10", 1, 1 >= 1);
    test("relational11", 0, 1 >= 5);
    test("relational12", 0, 4+6*7 < 46);
    test("relational13", 1, 4+6*7 <= 46);

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
    test("static_1", 10, case_static_1());

    test("array_1", 0, case_array_1());
    test("array_2", 2, case_array_2());
    test("array_3", 3, case_array_3());
    test("array_4", 5, case_array_4());
    test("array_access_1", 4, case_array_access_1());
    test("array_access_2", 4, case_array_access_2());
    test("array_access_3", 4, case_array_access_3());
    test("array_access_4", 4, case_array_access_4());
    test("array_access_5", 32, case_array_access_5());
    test("array_access_6", 62, case_array_access_6());

    test("gvar_1", 6, case_gvar_1());
    test("gvar_2", 14, case_gvar_2());
    test("gvar_3", 13, case_gvar_3());
    test("gvar_4", 31, case_gvar_4());

    test("char_array_1", 10, case_char_array_1());
    test("char_array_2", 7, case_char_array_2());

    test("case_str_1", 10, case_str_1());

    printf("========================\n");
    printf("All test cases passed!!!\n");
    return 0;
}