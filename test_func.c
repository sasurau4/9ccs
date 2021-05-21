#include <stdio.h>
#include <stdlib.h>

// Test function for function call
int shout() {
    printf("shout!\n");
    return 2;
}

int sum(int x, int y) {
    printf("x is %d\n", x);
    printf("y is %d\n", y);
    printf("the sum is %d\n", x + y);
    return x + y;
}

int *alloc1(int x1, int x2) {
    static int arr[2];
    arr[0] = x1;
    arr[1] = x2;
    return arr;
}

void allocp2(int **p, int x1, int x2) {
    static int arr[2];
    arr[0] = x1;
    arr[1] = x2;
    *p = arr;
    return;
}

void allocp4(int **p, int x1, int x2, int x3, int x4) {
    static int arr[4];
    arr[0] = x1;
    arr[1] = x2;
    arr[2] = x3;
    arr[3] = x4;
    *p = arr;
    return;
}

int test(char *test_name, int expected, int actual) {
    if (expected == actual) {
        printf("Test %s pass\n", test_name);
    } else {
        printf("====\n");
        printf("Test failed\n");
        printf("Expect: %d\n", expected);
        printf("Actual: %d\n", actual);
        exit(1);
    }
}