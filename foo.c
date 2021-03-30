#include<stdio.h>

// Test function for function call
int foo() {
    printf("shout from foo\n");
}

int bar(int x, int y) {
    printf("x is %d\n", x);
    printf("y is %d\n", y);
    printf("bar answer is %d\n", x + y);
}
