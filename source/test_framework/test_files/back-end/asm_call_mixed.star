int mixed(int a, float b, int c, char d, bool e, float f) {
    float bb = b / 2.0;
    int aa = a * 2 / c;
    float ff = f + .0023 + .0054;

    If aa == 24 && bb == 1.75 && d == '\n' && e == false && ff == 0.01 {
        Return 2;
    }
    Else {
        Return -1;
    }
}

/* Testing a func call with mixed type arguments.
 * Should return 2; */
int start() {
    Return mixed(123, 3.5, 10, '\n', false, 0.0023);
}
