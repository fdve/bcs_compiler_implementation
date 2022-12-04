### Software required
The following programs are needed to use the Star compiler.
1. Flex
2. Bison
3. A C++ compiler
4. Make
5. GCC
6. The Google Test Framework

### Install script
Run the get_depends.sh script to install all the dependencies needed for the compiler and for the compilers test framework.

### Tested
The compiler has been tested on a 5.10.16.3-microsoft-standard-WSL2 Linux kernel,
as well as also on an operatating system running Ubuntu 20.04.5 LTS.

The versions of the tools which were used during testing :
1. flex 2.6.4
2. bison 3.5.1
3. gcc and g++ 9.4
4. make 4.2.1
5. libgtest 1.10.0-2

### Language Reference
The compiler compiles source programs which are written in a toy language called Star. The reference for the Star language can be found the root directory, named language_reference_star.pdf. A couple of sample programs written in the Star language can be found in the root directory /star_examples.

### Compiling a Star program
To compile a Star program, the following command can be used:  
`make compiler; ./star_compiler test_file.star --d=star_output.s; gcc star_output.s; ./a.out;`  
This will compile the compiler, then feed the test_file.star to the compiled compiler.
The --d flag determines the location of the output Assembly, which in this case is a file named star_output.s in the same directory.
Afterwards the generated assembly is compiled with gcc, and finally the compiled assembly is executed.

`echo $?` can be used in bash after executing the compiled assembly to print the return value returned by the start function. Keep in mind that this value ranges from 0 - 255, but can be useful when checking the result of a simple calculation.

Other flags which can be supplied while compiling a program are -ast for printing the abstract syntax tree and -tac for printing the generated three-address code.


### Possible syntax errors
Keep in mind that all the Star language control flow statements begin with an uppercase character. Also, incrementing a variable like i++ is not currently supported by the compiler, therefore use i += 1 instead.

### Running the test framework
The test framework can be running the following command:   
`make gtest; ./run_gtest`  
This will compile the test framework and run the tests. The errors found in the front-end test files are emitted, and can be used to understand what type of errors is being looked for. 
