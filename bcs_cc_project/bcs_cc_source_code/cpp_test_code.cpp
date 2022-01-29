/******************************************************************************
                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    cout<<"Hello World";

    int i = 0;

    for(bool b = false; b == false; b = !b) {

        if(i == 5) {
            b = true;
        }

        std::cout << "hoi" << std::endl;
    }

    std::vector<int> list;
    list.push_back(0);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);

    int K = 0;

    if(true == 1) {
        std::cout << "true == 1" << std::endl;
    }

    for(bool i : list) {
        std::cout << "i = " << i << std::endl;
    }

    for(; i < list.size(); i++) {
        // std::cout << "i = " << list.at(i) << std::endl;
    }

    for(;;) {
        std::cout << "I AM STUCK :( " << std::endl;
        break;
    }

    for(int i = 0; i < 10; i++) {
        std::cout << "does this work? l0l" << std::endl;
    }

    // std::cout << typeid(10.).name() << std::endl;
    int scope_variable = 0 ;

    std::cout << "scope_variable = " << scope_variable << std::endl;
    {
        int scope_variable = 2;

        std::cout << "scope_variable = " << scope_variable << std::endl;

        // int scope_variable = 4;

        scope_variable = 5;

    }

    std::cout << "scope_variable = " << scope_variable << std::endl;
    return 0;
}

/*

for Int i = 0; i < 10; i++ {

}

If a < b

if a < b
    a

    if a < b then
        b++;
    endif;

If a < b
    // do stuff
Endof


For Int i = 0; i < 10; i++ {
    // do stuff
}

*/
