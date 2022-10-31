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

void fnc3() {
    // error : wrong return type 
    Return true == false;
}
