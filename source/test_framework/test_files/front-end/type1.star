/* File should give 26 errors. */

int start() {
    // error : wrong return type
    Return 12.34;
}

void fnc1() {
    // error : wrong return type
    Return 12.34;
}

float fnc2() {
    // error : no return value in non void function
    Return;
}

void fnc3(int x, float y, bool z) {}

void fnc4() {
    int xxxx = 10;

    // error : types invalid var def
    int A = 123.354;

    // error : types invalid assign
    int B = 20;
    B = false;

    // correct
    int a = start();

    // error : types invalid assign
    char c = fnc1();

    // error : types invalid bin op
    int d = false * 123.3;

    // error : types invalid bin op
    int e = 123 * 90.3;

    // error : types invalid unary op
    !'x';

    // error : types invalid unary op
    !(!(!3));

    // valid
    -(-(-(10 + - xxxx)));

    // error : types invalid unary op
    !(true * 1.2);

    // error : cannot apply - un op to a char
    -'c';

    // error : calling a symbol which is not a function
    int fake_function = 10;
    fake_function(123,23,4);

    // error : types invalid function params
    fnc3(10.0, 23.3, 'x');

    // valid
    fnc3(xxxx, 23.3, false);

    // error : invalid num of args provided
    fnc3(10, 10.0, true, 0);

    // error : invalid num of args provided
    fnc3();

    // error : invalid num of args provided
    start(1,2,3,4,5);

    // correct
    fnc3( 10, 10.2, true);

    // error : if conditional not bool
    If 'x' { }

    // error : cannot apply ! to int
    If (!(!(!(10)))) {}

    int j = 20;
    float z = 12.3;

    // error : assign diff type
    z = 90;

    int yyy = 30;
    int fyyy = 300;
    int tempf = 30;

    // error : incompatible types
    tempf = (z + (yyy * fyyy));

    // error : assign incompatible types
    int zzz = z;

    // valid
    float zz = z + z;

    // error : for conditional not bool
    For int i = 0; 123.345; i += 2; {}

    // valid
    For int i = 0; i < 20; i += 2; {} // For int i = 0; i < 20; i = i + 2; {}

    // error : while expression not bool
    While 13 * 13 {}

    // error : do while expression not bool
    Do {} While 12.3 + 23.6;

    // error : type invalid return type
    Return true;

    // valid
    Return;
}
