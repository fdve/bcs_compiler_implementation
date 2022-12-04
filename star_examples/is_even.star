bool is_even(int n) {
    If n % 2 == 0 {
        Return true;
    }
    Else {
        Return false;
    }
}

int start() {
    int input_num = 10;

    For int i = 0; i < 10; i += 1; {
        input_num += 1;
    }

    If is_even(input_num) {
        Return 1;
    }
    Else {
        Return 0;
    }
}
