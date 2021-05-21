int test(int expected, int actual) {
    if (expected == actual) {
        printf("Test pass with %d\n", expected);
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
    test(0, 0);
    test(42, 42);
    test(2, 1 + 1);
    test(25, 5 + 20);
    test(21, 5 + 20 - 4);
    test(41, 12 + 34 -5);
    test(47, 5+6*7);
    test(15, 5*(9-6));
    test(4, (3+5)/2);
    test(5, +5);
    test(10, -10+20);

    return 0;
}