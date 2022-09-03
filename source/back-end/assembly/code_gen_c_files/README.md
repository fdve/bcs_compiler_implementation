/* How to generate unoptimized assemnly?

The flags -O1 - -O3 optimize from little to most.
Use -O0 to not optimize (this is the default!).

https://gcc.gnu.org/onlinedocs/gcc-3.3.1/gcc/Optimize-Options.html

https://gcc.godbolt.org/#compilers:!((compiler:g530,options:%27-xc+-std%3Dgnu11+-Wall+-Wextra++-O3+-fverbose-asm+-ffast-math+-march%3Dhaswell%27,source:%27int+constants()+%7B+int+a+%3D+10,+b+%3D+20%3B+return+a+*+b%3B+%7D%0Aint+variables(int+a,+int+b)+%7B+return+a+*+b%3B+%7D%0A%27)),filterAsm:(commentOnly:!t,directives:!t,intel:!t,labels:!t),version:3
*/
