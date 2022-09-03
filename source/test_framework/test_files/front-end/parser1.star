// A simple, valid Start program. //

/* Multiline comment on a single line. */

/* Multiline
 * Comment
 * On multiple lines.
 */

 float _hello_() { Return 1.1; }

int       start   ()    {    Return    0   ;   }

void  test_void(  float  c  ,  float  b  ) {  {  }  }

void test_constants() { int a = 10; float b = 13.37; char c = 'A'; bool d = false; }


void test_operators_1() { int a = 10; a = 30; int b = 30; a == b; a != b; a && b; a || b; }

void test_operators_2() { int a = 2; int b = 4; a < b; a > b; a <= b; a >= b; a + b; a - b; a * b; a / b; a % b;}

float test_empty_1() { }

float test_empty_2() { { { { } } } }

float test_empty_3(int a, bool b, float c, char d, int e) {   }

bool test_bool(bool a, bool b, bool c, bool d) { Return a || b || c || d; }

char test_var_def(int b, int c) { float f = 13.37; }

char test_var_assign_1() { int birth_year = 1997; birth_year = 2022; }

char test_var_assign_2() { int weight = 10; float increase = 0.5; weight += 20; weight /= increase; }

char test_stmnt_expr() { float a = 30.4; 123.1233; }

float test_cf_1() {
    int increase_me = 0;
    While 123.456 {
        increase_me += 10;
    }
}

void test_cf_2() {
    int a = 0;
    If 123 {
        a = 2;
        char c = 'c';

        If 123 { a = 2; } Else { a = 3; }
    }
    Else { }
    Return;
}

void test_cf_3() {
    For int i = 0; i < 10; i = (i + 3 * 6); {
        float myfloat = .12345;
    }
}

void test_cf_4() {
    bool a = false; bool b = false; bool c = false; bool d = false;
    If a > (b + c) {
        a = b && c;
    }
    Else {
        For int i = 0; i < 10; i = i + 1; {
            If(!a && (a > (b + c)) && test_bool(!a, b, c, d)) {
                Do { (((a + b))); } While a < b;
            }
        }
    }
}

void test_call_me(int a, int b, int c) {}

int test_call_me_2(int a) { Return a; }

int test_call_me_3() { Return 666666; }

int test_call_me_4(int a, int b, int c, int d, int e) { Return 55555; }

void test_fnc_call(int a, float b) {
    char bb = '\n';
    char bb = 'a';
    int a = 1;
    int b = 2;

    test_cf_4(); test_call_me(1, 2, 3); test_call_me(a, b, test_call_me_2(1));
    Return test_call_me_4(1,2,3,4, 5 + test_call_me_2(test_call_me_3()));
}

void test_unary() {
    float a = 2; float b = 3; bool c = false;
    a - b;
    a = -a;
    -a;
    c = !c;
    a + (-a);
    a + - a;
    -(a) + b;
    -a + b;
    a - -b;
}
