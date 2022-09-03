.LC0:
        .long   1068121457

.globl main

main:
        pushq   %rbp
        movq    %rsp, %rbp
        movss   .LC0(%rip), %xmm0
        movss   %xmm0, -4(%rbp)
        movl    $0, %eax
        popq    %rbp
        ret
