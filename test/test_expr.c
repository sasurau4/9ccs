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
    // Test arithmetic ope
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

    // Test relational operators
    test(1, 2 == 2);
    test(0, 12 == 2);
    test(1, 100 != 1);
    test(0, 100 != 2 * 50);
    test(1, 1 < 2);
    test(0, 1 < 1);
    test(0, 1 > 1);
    test(0, 2 <= 1);
    test(1, 1 <= 1);
    test(1, 1 >= 1);
    test(0, 1 >= 5);
    test(0, 4+6*7 < 46);
    test(1, 4+6*7 <= 46);

    return 0;
}