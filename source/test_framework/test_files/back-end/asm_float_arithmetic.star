int start() {
    Return 1;
}

/* Should return -2.3 */
float test_asm_float_arithmetic() {
    float f1 = 3.5;
    float f2 = 1.75;
    float f3 = f1 / f2 * 2.5 + 6.6 - 13.9;
    Return f3;
}
