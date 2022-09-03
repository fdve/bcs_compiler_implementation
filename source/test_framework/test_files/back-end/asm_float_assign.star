int start() {
    Return 1;
}

/* Test float var declaration, assignment, and return value.
* Function should return the value 123.356. */
float test_asm_float_assign() {
    float f1 = 12.34;
    float f2 = -0.0;
    float f3 = 0.0;
    float f4 = f1;

    f3 = 19.97;
    f1 = f2;
    f2 = f3;
    f3 = f4;

    Return f3;
}
