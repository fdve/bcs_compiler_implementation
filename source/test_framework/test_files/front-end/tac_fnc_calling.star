int f (int x, int y, int z) { Return x + y + z; }

int f2 (int x, int y) { Return x + y; }

int f3() { Return 1; }

int start() {
    /* Function calling. */
    f(f2(1,f2(111,222 + 30 * 2)),20,30);

    // f2(1,2);
    // f3();
    // int a = f4();

    f3();


    Return 0;
}
