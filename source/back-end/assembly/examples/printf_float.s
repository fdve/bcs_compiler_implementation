

.globl main

.LC0:
        .long   1068121457 # hexadecimal  0x3FAA3D71 also works fine

.LC1:
        .string "myfloat = %f\n"


main:
        pushq   %rbp
        movq    %rsp, %rbp
        subq    $16, %rsp
        movss   .LC0(%rip), %xmm0
        movss   %xmm0, -4(%rbp)
        pxor    %xmm1, %xmm1
        cvtss2sd        -4(%rbp), %xmm1
        movq    %xmm1, %rax
        movq    %rax, %xmm0
        movl    $.LC1, %edi
        movl    $1, %eax
        call    printf
        movl    $123, %eax
        leave
        ret
