#include <stdio.h>

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
