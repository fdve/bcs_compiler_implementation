.globl _start

# original code :
#   int b = 10;
#   int a = 9 + b;

_start:
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $10, -4(%rsp)       # int b = 10;
        movl    $9,   %edx          # store 9 temporary in rdx
        addl    -4(%rsp),   %edx    # add a (10) to rdx, storing 19 there now (b val)
        movl    %edx, -8(%rsp)      # move 19 to a
        movl    -8(%rsp), %eax      # store variable a in rax, check with echo $?
        movq    %rbp, %rsp
        popq    %rbp
        ret
