void fnc3(int x, float y, bool z) {}

int start() {
    int xxxx = 10;

    // error : types invalid function params
    fnc3(10.0, 23.3, 'x');

    // correct
    fnc3(xxxx, 23.3, false);

    // correct
    fnc3(10, 10.2, true == false);
}
