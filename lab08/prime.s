  # File: prime.s
  # Purpose: Read a positive integer from standard input and determine if it is prime.
  # Author: Sean Balbale
  # Date: 10/22/2025


.LC0:
  .string "Enter a positive integer: "
.LC1:
  .string "%d"
.LC2:
  .string "%d is a prime number.\n"
.LC3:
  .string "%d is not a prime number.\n"

.global main
main:
  pushl %ebp          # set up stack frame
  movl %esp, %ebp     # set up stack frame
  subl $8, %esp       # allocate space for integer input (8 bytes for alignment)

  # prompt user for input
  pushl $.LC0         # push address of prompt string
  call printf         # print prompt
  addl $4, %esp       # clean up stack
  
  # read integer from user
  leal  -4(%ebp), %eax     # address of first local variable
  pushl %eax               # push address to store input
  pushl $.LC1              # push format string
  call  scanf              # read integer
  addl  $8, %esp           # clean up stack (2 arguments = 8 bytes)
 
  movl  -4(%ebp), %eax     # load input integer into eax
  
  # handle special cases
  cmpl  $2, %eax           # compare input with 2
  jl    notPrime           # if input < 2, it is not prime
  je    isPrime            # if input == 2, it is prime
  
  # check if even
  movl  %eax, %ebx         # save number in ebx
  andl  $1, %eax           # check if odd (eax & 1)
  jz    notPrime           # if even (and not 2), not prime
  
  movl  %ebx, %eax         # restore number to eax
  movl  $3, %ebx           # divisor starts at 3

checkLoop:
  # check if divisor^2 > number (if so, number is prime)
  movl  %ebx, %ecx         # copy divisor to ecx
  imull %ecx, %ecx         # square the divisor
  cmpl  %ecx, %eax         # compare number with divisor^2
  jl    isPrime            # if number < divisor^2, it's prime
  
  # save number before division
  pushl %eax               # save number on stack
  
  # perform division
  cltd                     # sign-extend eax to edx:eax for division
  divl  %ebx               # divide eax by ebx
  
  # check remainder
  cmpl  $0, %edx           # check if remainder is 0
  popl  %eax               # restore number from stack
  je    notPrime           # if remainder is 0, not prime
  
  addl  $2, %ebx           # increment divisor by 2 (only check odd divisors)
  jmp   checkLoop          # continue loop

isPrime:
  movl  -4(%ebp), %eax     # reload original number
  pushl %eax               # push number
  pushl $.LC2              # push format string
  call printf              # print prime message
  addl  $8, %esp           # clean up stack
  jmp   .done

notPrime:
  movl  -4(%ebp), %eax     # reload original number
  pushl %eax               # push number
  pushl $.LC3              # push format string
  call printf              # print not prime message
  addl  $8, %esp           # clean up stack

.done:
  leave
  ret

