# Program: getbyte.s
# Purpose:
# Author: Sean Balbale
# Date: 10/8/2025

# Write an assembly program (getbyte.s) which will extract byte n 
# from a 4-byte word x. Here n is 0, 1, 2, or 3, and byte 0 is the 
# least significant byte. For example, if x = 0x12345678 and n = 1, 
# the output should be 0x56. Assume that the values of x and n are hardcoded.
# Make sure your program works with all possible values of n. Don’t forget 
# to comment your code. Hint: Use shift instructions but note that the 
# source operand for shift instructions has to be either an immediate or the %cl register.
# Your program should output like this when n = 1:
# Byte 1 of 0x12345678 is 0x56.

.LCO:
	.string	"Byte %d of 0x%x is 0x%x\n"

  .globl  main
main:
  pushl %ebp # save frame pointer
  movl %esp,%ebp # adjust stack pointer
  

  movl $0x12345678, %eax # load x into eax
  movl %eax, %edx         # save x in edx for later
  movl $1, %ebx           # load n into ebx
  movl %ebx, %esi         # save n in esi for later
 # imull $8, %ebx, %ebx    # multiply n by 8 to get bit shift amount (n * 8)

  sall $3, %ebx           # shift mult by 8
  movb %bl, %cl           # move n (lower 8 bits of ebx) to cl
  shrl %cl, %eax          # shift right eax by n
  andl $0xFF, %eax        # mask to get the least significant byte

  pushl %eax              # push extracted byte
  pushl %edx              # push x
  pushl %esi              # push n
  pushl $.LCO             # push address of format string
  call printf
  

  leave # restore the current activation
  ret # return to caller
