### Software required
The following programs are needed to use the Star compiler. 
1. Flex
2. Bison
3. C++ 
4. Make. 


The following versions were used to test it 
....
The compiler has been tested on 5.10.16.3-microsoft-standard-WSL2 Linux kernel
and on (test on linux standalone version here compiler here)

(Also make sure that both test platforms had the same bison flex c++ make etc) 



### Language Reference 
The compiler compiles for a toy language called Star. The reference for this language can be find the root git directory, named langref_star.pdf.





The Compiler can be compiled using the make command, followed
by passing a Star source file to the compiled binary : 
make;
./star_compiler my_star_source_file.star 

Sample star source files can be find in the ./source/test_framework/tests directroy

Compiling required gcc?, make, flex and bison, 



The /source directory is where the source code of the compiler resides.
Each subdirectory within the source directory has its own README file.

