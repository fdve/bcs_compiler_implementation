# global data  #
    .data
format: .asciz "%d\n"
.text
    .global main
main:
  push  %rbx
  lea   format(%rip), %rdi
  mov   $1, %esi           # Writing to ESI zero extends to RSI.
  xor   %eax, %eax          # Zeroing EAX is efficient way to clear AL.
  call  printf
  pop   %rbx
  ret
