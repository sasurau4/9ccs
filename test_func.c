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
