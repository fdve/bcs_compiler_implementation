int start() {
    Return 1;
}

/* Test function should return 13.37. */
float test_asm_float_cf() {
    float f = 1.25;
    float f2 = 2.5;
    float f3 = 2.55;

    If f != 1.25 && f + f2 != 3.75 && f3 >= 2.65 {
        // something went wrong
        Return 1.0;
    }

    Do {
        f += 2.0;
        While f <= 9.25 {
            f += 2.0;
        }
    } While f < 19.25;


    // f should be 19.25 at this point
    For int i = 0; i < 1; i += 1; {
        f *= 2.0;
    }

    // f should be 38.5 at this point
    If(f == 38.5) {
        bool b = f > 38.5;

        If(!b && f / 2.0 == 19.25) {
            // success!
            Return 13.37;
        }
        Else {
            // something went wrong
            Return 2.0;
        }
    }
    Else {
        // something went wrong
        Return 3.0;
    }
}
