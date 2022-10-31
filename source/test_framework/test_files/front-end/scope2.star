void f() {}

int start() {
    int a = 10;
    a = 13;
    {
        int b = 20;
        b = 30;
    }
    int c = 30;
    f();
    Return 0;
}
