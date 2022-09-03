/*
Test these?
    (1) x = y op z
    (2) x =   op y
    (3) x = y
    (4)     goto L
    (5) if x goto L
    (6) if x relop y goto L
    (7) param for func param
    call p, n
    y = call p, n
    return y
*/

int start() {
    int a = 10;
    int b = 20;
    int c = a + b;
    a + 5;
    Return 5;
}

float f() { Return 2.0; }

/*
void cfIf() {
    If 33 < 44 && 55 > 66 || 77 <= 88 {

        If a == b {}

    }
}
*/

// -[] testing nullptr entries / unknown variables encoutnered in expressions,
// make new test of? 

void fff() {
    int a = b;

    assign_unknown = assign_unknown_result;

    xxx();

    yz == zy;

    Return c;
}











// If 33 < 44 || 55 > 66 && 77 <= 88 {
// If (33 < 44) || (55 > 66) {
