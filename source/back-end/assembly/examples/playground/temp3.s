.globl main


main:
    pushq   %rbp
    movq    %rsp, %rbp
    movb    $99, -1(%rbp)
    movl    $20, 11111111(%rbp)
    nop
    popq    %rbp
    ret
