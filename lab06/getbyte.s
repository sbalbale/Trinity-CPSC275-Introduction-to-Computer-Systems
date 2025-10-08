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
	.string	"Byte %d of %u is %u\n"

  .globl  main
main:
  pushl %ebp # save frame pointer
  movl %esp,%ebp # adjust stack pointer
  
  movl $0x12345678, %eax # load x into eax
  movl $1, %ebx # load n into ebx 
  pushl %ebx
  pushl %eax
  
  sarl %ebx, %eax # shift right eax by n
  andl $0xFF, %eax # mask to get the least significant byte

  pushl %eax
  pushl .LCO
  call printf

  leave # restore the current activation
  ret # return to caller
