/* Testing integer control flow, do while, while, for, if
 * with boolean expressions containing <= == >= != && ||.
 * start() should return 111.  */

int start() {
    int a = 0;
    int b = -13;

    For int i = 0; i < 10; i += 1; {
        a = a + 1;
    }

    // a should be 10 at this point
    If a != 10 || b != -13 || 3 != 3 {
        // something went wrong
        Return 1;
    }

    While(a >= 10 && a <= 150) {
        a += 1;
    }

    // a should be 151 at this point
    Do {a -= 2; } While a > 101;


    // a should be 101 at this point
    If a == 101 {
        While a < 111 {
            a += 1;
        }

        // a should be 111 at this point
        If a != 111 {
            // something went wrong
            Return 1;
        }
        Else {
            // success! should get here
            Return a;
        }
    }
    Else {
        // something went wrong
        Return 1;
    }

    // something went wrong
    Return 1;
}
