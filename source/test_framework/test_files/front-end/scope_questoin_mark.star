int start() { Return 0; }

void scope_1(int b, int c) {
    int func_locals_a = 10;
    int func_locals_d = 30;

    {
        {

            {
                int innervar = 900;
            }
            int innervar = 900;
        }
    }
}

void scope_2(int XX, int YY) {
    {
        {
            For int i = 0; i < 10; i += 1; {
                XX = 200;
                int CCC = 300;
                {
                    int yyyy = 300;
                    If(1) {
                        int aaa = 400;
                    }
                    int aaa = 400;

                }
                int yyyy = 3000;
            }

            For int i = 0; i > 10; i -= 4; {
                // do nothing
            }

            int CCC = 300;
        }
    }
    int CCC = 200;
}

int f(int x) { Return x; }


/*
void stuff(int x) {
    int i = 0;

    While i < 10 {
        i += 1;

        int b = 10;
        {
            b = 20;
            int c = i + x;
        }
        int c = 30;

    }
    int c = 40;
}
*/
