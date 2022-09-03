.globl main

main:
    pushq           %rbx
    subq $0x18,     %rsp # Allocate 24 bytes of space on the stack
    addq $0x18,     %rsp # Give back 24 bytes of stack space
    pop             %rax

    ret
