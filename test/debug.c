int case_array_2() {
    // int a[2];*(a+1)=2;return *(a+0);
    static int a[1];*a=2;return *a;
}

// int case_ptr_plus_2() {
//     // From test_func
//     int *p = alloc1(3,6);
//     return *(p+1);
// } 

// int piyo1() {
//     // int index = 0;
//     char a[1]; a[0]=10; return a[0];
// }
// int piyo2() {
//     return 15;
// }
int main() {
    // return 3;
    // char a[1]; a[1]=10; a[0];
    // return piyo1();
    // int a[2];*(a+1)=2;return *(a+0);
    // int a[2];*a=2;return *a;
    // return case_ptr_plus_2();
    return case_array_2();
}
