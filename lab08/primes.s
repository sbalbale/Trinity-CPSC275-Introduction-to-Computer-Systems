  # File: primes.s
  # Purpose: Read a positive integer N and print all primes less than N.
  # Author: Sean Balbale
  # Date: 10/22/2025


.LC0:
  .string "Enter a positive integer N: "
.LC1:
  .string "%d"
.LC2:
  .string "Primes less than %d:\n"
.LC3:
  .string "%d\n"

.globl main
main:
  pushl %ebp          # set up stack frame
  movl %esp, %ebp     # set up stack frame
  subl $8, %esp       # allocate space for N and counter (8 bytes for alignment)

  # prompt user for input
  pushl $.LC0         # push address of prompt string
  call printf         # print prompt
  addl $4, %esp       # clean up stack
  
  # read integer from user
  leal  -4(%ebp), %eax     # address of N
  pushl %eax               # push address to store input
  pushl $.LC1              # push format string
  call  scanf              # read integer
  addl  $8, %esp           # clean up stack (2 arguments = 8 bytes)
  
  # print header
  movl  -4(%ebp), %eax     # load N
  pushl %eax               # push N
  pushl $.LC2              # push format string
  call printf              # print header
  addl  $8, %esp           # clean up stack
  
  # initialize counter to 2 (first prime)
  movl  $2, -8(%ebp)       # counter = 2

primeLoop:
  # check if counter < N
  movl  -8(%ebp), %eax     # load counter
  cmpl  -4(%ebp), %eax     # compare counter with N
  jge   .done              # if counter >= N, exit loop
  
  # call isprime with counter as argument
  pushl -8(%ebp)           # push counter as argument
  call isprime             # call isprime function
  addl  $4, %esp           # clean up stack (1 argument = 4 bytes)
  
  # check return value
  cmpl  $1, %eax           # check if isprime returned 1
  jne   nextNumber         # if not prime, skip printing
  
  # print the prime number
  pushl -8(%ebp)           # push counter
  pushl $.LC3              # push format string
  call printf              # print prime
  addl  $8, %esp           # clean up stack

nextNumber:
  incl  -8(%ebp)           # increment counter
  jmp   primeLoop          # continue loop

.done:
  leave
  ret

# isprime function
# Parameter: positive integer on stack at 8(%ebp)
# Returns: 1 if prime, 0 if not prime in %eax
isprime:
  pushl %ebp               # set up stack frame
  movl  %esp, %ebp         # set up stack frame
  pushl %ebx               # save callee-saved register
  pushl %ecx               # save callee-saved register
  
  movl  8(%ebp), %eax      # load parameter into eax
  
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
  pushl %edx               # save edx
  
  # perform division
  cltd                     # sign-extend eax to edx:eax for division
  divl  %ebx               # divide eax by ebx
  
  # check remainder
  cmpl  $0, %edx           # check if remainder is 0
  popl  %edx               # restore edx
  popl  %eax               # restore number from stack
  je    notPrime           # if remainder is 0, not prime
  
  addl  $2, %ebx           # increment divisor by 2 (only check odd divisors)
  jmp   checkLoop          # continue loop

isPrime:
  movl  $1, %eax           # return 1 (prime)
  jmp   returnIsPrime

notPrime:
  movl  $0, %eax           # return 0 (not prime)

returnIsPrime:
  popl  %ecx               # restore callee-saved register
  popl  %ebx               # restore callee-saved register
  leave
  ret

