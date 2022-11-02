int start() {
    // should evaluate to 11
    int a = 1 - 2 * -(-(-3)) + 4;

    // after this a should be 25
    a += a + 3 / -(4 + -5);

    // < > >= <= has higher prec than != ==, has higher prec than &&, higher prec than ||

    // testing prec && is greater than prec ||, b should be true
    bool b = true || false && false;

    // testing prec !=  is greater than prec &&, c should be false
    bool c = true != false && false;

    // testing prec >= is greating than ==, d should be false
    bool d = false == false >= false;

    If a == 25 && b == true && c == false && d == false {
        // success
        Return 0;
    }
    Else {
        // something went wrong
        Return 1;
    }

    // should never come here
    Return 2;
}
