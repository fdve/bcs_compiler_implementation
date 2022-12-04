The test framework makes use of the GoogleTest framework https://github.com/google/googletest.

The input tests are located in the front-end and back-end subdirectory (there is currently no middle-end testing).

### Front-end testing
Front-end testing works by  
1. either parsing the test file and checking if parsing errors occured.
2. parsing and then performing semantic analysis, checking e.g whether any type incompatibility or function call paramater mismatch errors happened.

The test checks whether the amount of expected error messages are generated. Also check whether the generated errors are the same as the expected errors.

### Back-end testing
To test the compilers generated Assembly, first the Star compiler has to generate Assembly files from the test input files, this happens in gtest_asmfilegen.cpp.

Following, in gtest_main.cpp the gtests are compiled together with the generated Assembly files, where the test functions in gtest_main.cpp will be calling and executing the compiled generated Assembly functions and compares their return results to the expected return results for those functions.

The generated Assembly files are put in the /test_files/gen_asm_files directory. The mandatory start() method from the Star source file gets translated to a test_x function label, with a .globl prefix so that the function will be accessible from outside the file and can be found by the gtest to execute.

For compiled Assembly files which return a float value, the source Star files will contain a start method and a seperate test method which returns the float value. The .globl directive will be followed by the name of the seperate test method so that the function can be called and tested from the gtest.
