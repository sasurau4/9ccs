int main() { 
    // This is comment!
    /* This is block comment!
    */
    // Test arithmetic ope
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

    return 0;
}