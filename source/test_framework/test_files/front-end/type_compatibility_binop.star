int start() {
    // error : types invalid bin op
    bool d = false * 123.3;

    // error : types invalid bin op
    int e = 123 * 90.3;

    // error : types invalid unary op
    !(true - 1.2);

    float f = 1.2;
    char c = 'c';

    // error : incompatible types
    char tempf = c / f;

    bool mod = true % false;

    Return 0;
}
