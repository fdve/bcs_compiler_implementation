.globl main


# -----------------------------------------------------------------------------
# int main() {

#   push a 32 int var named _i? with val 1 on the stack?
#   int i = 1;
#   push a 32 int var named _j? with val 2 on the stack?
#   int j = 2;
#   int k = i + j;
# }
#
# Assemble and Link:
#     gcc fib.s
# -----------------------------------------------------------------------------


main:

    # put 1 (1 byte int/char) into accumulator register
    mov     $1,     %eax

    # add 2 (1 byte int/char), storing result in accumulator
    add     $4,     %eax

    # eax or rax? if I do  mov     %eax / %rax,     $3 we get segment fault
    # mov     $4,     %edx

#    loopstart:
#                inc %eax
#                dec %cx
#                jnz loopstart

    ret
