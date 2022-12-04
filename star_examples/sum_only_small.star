float sum_only_small(float a, float b, float c) {
    If a > 10.0 && b > 10.0 && c > 10.0 {
        Return 0.0;
    }
    Else {
        Return a + b + c;
    }
}

int start() {
    float a = 4.123;
    float b = 6.123;
    float c = 8.123;

    float result = sum_only_small(a, b, c);

    If(result == 0.0) {
        Return 1;
    }

    If(result != 18.369) {
        Return 2;
    }

    Return 3;
}
