### Install script

Maybe add easy automatic install script here?

### Software required
The following programs are needed to use the Star compiler.
1. Flex
2. Bison
3. A C++ compiler
4. Make.
5. (for gtest) GCC for the test framework
6. (for gtest) The google test framework

The root directoy contains an get_depends.sh script which will install all the required dependencies for the compiler itself and for the tets framework of the compiler.

[] test compiler on MacOS
[] test compiler on WSL2
[] test compiler on Ubuntu distro itself

The compiler has been tested with the program versions stated above.
Newer versions should most likely also work, older versions may not.
The compiler has been tested on 2 platforms. The first platform
was a 5.10.16.3-microsoft-standard-WSL2 Linux kernel,
and the second platform a (test on linux standalone VM version here compiler here)

(Also make sure that both test platforms had the same bison flex c++ make etc)

[] Add a script which installs all programs automatically?

### Language Reference
The compiler compiles source programs which are written in a toy language called Star. The reference for this language can be find the root git directory, named langref_star.pdf. A couple of sample programs written in the Star language can be found in directory Y.

### Compiling a Star program

`make compiler`

### Common mistakes
The star language has grammer in which all control flow statements begin with a uppercase character. Therefore,
a common mistake would be to make if (a > b) or if a > b. instead of If a > b

to pitfills : i++ is not present, use i+=1 instead;

### Running the test framework

`make compiler` // compiles the Star compiler


The Compiler can be compiled using the make command, followed
by passing a Star source file to the compiled binary :
make;
./star_compiler my_star_source_file.star

Sample star source files can be find in the ./source/test_framework/tests directroy

Compiling required gcc?, make, flex and bison,

The /source directory is where the source code of the compiler resides. Each subdirectory within the source directory has its own README file, describing the directories purpose.
