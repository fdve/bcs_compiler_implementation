void fnc1() {}

int start() {
    // error : types invalid var def
    int A = 123.354;

    // error : types invalid assign
    int B = 20;
    B = false;

    // error : types invalid assign
    char c = fnc1();

    // error : types invalid assign
    int d = 123.345;

    // error : types invalid unary op
    !'x';

    // error : types invalid unary op
    !(!(!3));

    // error : if conditional not bool
    If 'x' { }

    // error : cannot apply ! to int
    If (!(!(!(10)))) {}

    float z = 12.3;
    int yyy = 30;
    int fyyy = 300;
    int tempf = 30;

    // error : assign diff type
    z = 90;

    // error : incompatible types
    tempf = z;

    // error : assign incompatible types
    int zzz = z;

    // error : for conditional not bool
    For int i = 0; 123.345; i += 2; {}

    // error : while expression not bool
    While 13 * 13 {}

    // error : do while expression not bool
    Do {} While 12.3 + 23.6;

    // correct
    int a = start();

    // correct
    -(-(-(10 + - a)));

    // correct
    For int i = 0; i < 20; i += 2; {} // For int i = 0; i < 20; i = i + 2; {}

    Return 0;
}
