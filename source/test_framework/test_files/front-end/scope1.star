int start() {
    a = 20;
    int c = 3;
    int d = 13;
    {
        {
            c += 3;
            int b = 10;
            int d = 133;
        }
        c += 2;
        b += 2;
    }
    Return 0;
}

void f1() {
    float start = 123.34;
    int b = start();
}
