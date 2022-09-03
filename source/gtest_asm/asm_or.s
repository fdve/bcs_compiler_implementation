# Need to make it global so it can be accessed in another file with extern
.globl asm_or

# Cannot hurt to define it as a function type, sometimes useful for dynamic linking, see comments in: https://stackoverflow.com/questions/65837016/how-to-call-a-function-in-an-external-assembly-file#comment116408928_65837016
.type asm_or, @function

asm_or:
    # int integrate(int from /*EDI*/,  int to /*ESI*/)

    # INPUT:
    #   the first parameter `from` is contained in %edi, the int-sized low half of %rdi
    #   the second parameter `to`  is contained in %esi

    # OUTPUT:
    #   return is passed in %eax;
    #      you can leave garbage in the high half of RAX if convenient


    movl $13, %eax

    ret
