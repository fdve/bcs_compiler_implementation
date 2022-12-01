/* Testing bool func call.
 * Should return 123. */


/* Should return true, if given param c is false */
bool ret_b(bool a, bool b, bool c) {
    a = true;
    b = true;
    c = !c;

    bool bb = false;
    bool bbb = false;
    bb = a && b && c && !bbb;
    Return bb;
}

int start() {
    bool b = ret_b(false, false, false);

    If b {
        Return 123;
    }
    Else {
        Return 1;
    }
 }
