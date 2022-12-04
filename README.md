
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
[] test compiler on MacOS? Of laten we dat zitten?
[x] test compiler on WSL2
[x] test compiler on Ubuntu distro itself
    [x] ran the get_depends, gtest framework worked, but might need a clean ubuntu install  
    [x] now test some sample programs

The compiler has been tested with the following program versions :
flex 2.6.4
bison 3.5.1
gcc and g++ 9.4
make 4.2.1
libgtest 1.10.0-2

The compiler has been tested on a 5.10.16.3-microsoft-standard-WSL2 Linux kernel
inside of Windows 10,
as well as also on an operatating system running Ubuntu 20.04.5 LTS,

and the third platform (maybe test on MacOs? )

(test on linux standalone VM version here compiler here)

[x] make sure that both test platforms had the same bison flex c++ make etc

### Compiler interface
Should we talk about the supported flags etc?

### Language Reference
The compiler compiles source programs which are written in a toy language called Star. The reference for this language can be find the root git directory, named langref_star.pdf. A couple of sample programs written in the Star language can be found in directory Y.

### Compiling a Star program
To compile a Star program, the following command can be used :
make compile; ./star_compiler my_star_file.star --d=star_output.s; gcc star_output.s -o; ./a.out; echo $?
This will compile the compiler, then feed the my_star_file.star to the compiler together
with the --d  flag determining the location of the output file to be in the same
directory with the name star_output.s, afterwards compiling the generated assembly
with gcc, and finally executing the compiled program.

### Common mistakes
The Star language control flow statements all begin with an uppercase character. Therefore,
a common mistake would be to make if (a > b) or if a > b instead of If a > b or If a > b.

Another thing to keep in mind that i++ is not currently supported by the compiler,
use i += 1 instead.

### Running the test framework
`make compiler` // compiles the Star compiler

The Compiler can be compiled using the make command, followed
by passing a Star source file to the compiled binary :
make;
./star_compiler my_star_source_file.star

Sample star source files can be find in the ./source/test_framework/tests directroy

Compiling required gcc?, make, flex and bison,

The /source directory is where the source code of the compiler resides. Each subdirectory within the source directory has its own README file, describing the directories purpose.
