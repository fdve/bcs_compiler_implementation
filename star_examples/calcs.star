int start() {
    int a = 2000;
    int b = 20001;
    int c = 200002;

    float d = 13.87;
    float e = -99.00;

    float result1 = d + e;
    int result2 = b % a;
    int result3 = c / b;

    If result1 != -85.13 {
        Return 0;
    }

    If result2 != 1 {
        Return 0;
    }

    Return c / b;
}
