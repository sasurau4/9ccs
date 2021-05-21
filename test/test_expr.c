int test(int expected, int actual) {
    if (expected == actual) {
        printf("Test pass\n");
    } else {
        printf("====\n");
        printf("Test failed\n");
        printf("Expect: %d\n", expected);
        printf("Actual: %d\n", actual);
        exit(1);
    }
}
int main() { 
    // This is comment!
    /* This is block comment!
    */
    test_expr(0, 0);
    test_expr(42, 42);
    test_expr(2, 1 + 1);
    test_expr(25, 5 + 20);
    test_expr(21, 5 + 20 - 4);
    test_expr(41, 12 + 34 -5);

    return 0;
}