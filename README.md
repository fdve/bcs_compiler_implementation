### Software required
The following programs are needed to use the Star compiler. 
1. Flex
2. Bison
3. C++ 
4. Make. 
5. we use gcc also right? For compiling the tests?

The compiler has been tested with the program versions stated above on 
2 platforms. Newer versions should most likely also work, older versions may not.
However, no guarantees here. 

The compiler has been tested on 5.10.16.3-microsoft-standard-WSL2 Linux kernel
and on (test on linux standalone VM version here compiler here)

(Also make sure that both test platforms had the same bison flex c++ make etc) 

### Language Reference 
The compiler compiles source programs which are written in a toy language called Star. The reference for this language can be find the root git directory, named langref_star.pdf.

### Compiling a sample program

### Compiling your own program 

### Common Pitfalls
The star language has grammer in which all control flow statements begin with a uppercase character. Therefore,
a common mistake would be to make if (a > b) or if a > b. instead of If a > b 

### Running the test framework 

`make compiler` // compiles the Star compiler 

  
The Compiler can be compiled using the make command, followed
by passing a Star source file to the compiled binary : 
make;
./star_compiler my_star_source_file.star 

Sample star source files can be find in the ./source/test_framework/tests directroy

Compiling required gcc?, make, flex and bison, 

The /source directory is where the source code of the compiler resides.
Each subdirectory within the source directory has its own README file.

