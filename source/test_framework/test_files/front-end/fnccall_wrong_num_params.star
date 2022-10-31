void fnc3(int x, float y, bool z) {}

int start() {
    // error : invalid num of args provided
    fnc3(10, 10.0, true, 0);

    // error : invalid num of args provided
    fnc3();

    // error : invalid num of args provided
    start(1,2,3,4,5);

    Return 0;
}
