  # File: prime.s
  # Purpose: Read a positive integer from standard input and determine if it is prime.
  # Author: Sean Balbale
  # Date: 10/22/2025


  # Purpose: Read a positive integer from standard input and
  # Author: Sean Balbale
  # Date: 10/22/2025

.LC0
  .string "Enter a positive integer: "
.LC1
  .string "%d"
.LC2
  .string "%d is a prime number.\n"
.LC3
  .string "%d is not a prime number.\n"

.global main
main:
  pushl %ebp          # set up stack frame
  movl %esp, %ebp     # set up stack frame
  subl $4, %esp       # allocate space for integer input

  #prompt user for input
  pushl $.LC0         # push address of prompt string
  call printf         # print prompt
  addl $4, %esp       # clean up stack
  
  # read integer from user
  leal  -4(%ebp), %eax     # address of first local variable
  pushl %eax               # push address to store input
  pushl $.LC1              # push format string
  call  scanf              # read integer
  addl  $4, %esp           # clean up stack without pop since value should be discarded
  
  call checkPrime             # call isPrime function

  leave
  ret

checkPrime:
  movl  -4(%ebp), %eax     # load input integer into eax
  cltd                     # sign-extend eax to edx:eax for division
  divl $2                  # divide eax by 2
  jz  not_prime            # if input is even, it's not prime
  pushl -4(%edp)         # push input integer
  pushl $.LC2             # push prime format string
  call printf             # print prime message
  addl $8, %esp           # clean up stack
  ret
  call .done


not_prime:
  pushl  -4(%ebp)         # push input integer
  pushl $.LC3             # push not prime format string
  call printf             # print not prime message
  addl $8, %esp           # clean up stack
  ret

.done:
  ret

