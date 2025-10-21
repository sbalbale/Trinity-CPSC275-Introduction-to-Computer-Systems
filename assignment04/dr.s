# File: dr.s
# Program: dr.s
# Purpose: Compute the digital root of a positive integer using recursion
# Author: Sean Balbale
# Date: 10/24/2025
#
.LC0:
    .string "Enter a positive number: "
.LC1:
    .string "%d"
.LC2:
    .string "\nThe digital root of %d is: %d\n"

.globl main 
main:
    pushl %ebp               # set up stack frame
    movl  %esp,%ebp          # set up stack frame
    subl  $8,%esp            # allocate space for two integers
    
    # prompt user for input
    pushl $.LC0              # push address of prompt
    call  printf             # print prompt
    addl  $4, %esp           # clean up stack without pop since value should be discarded
    
    # read integer from user
    leal  -4(%ebp), %eax     # address of first local variable
    pushl %eax               # push address to store input
    pushl $.LC1              # push format string
    call  scanf              # read integer
    addl  $8, %esp           # clean up stack without pop since value should be discarded
    
    # call dr function
    pushl -4(%ebp)           # push the input number
    call  dr                 # call digital root function
    addl  $4, %esp           # clean up stack without pop since value should be discarded
    
    # print result
    pushl %eax               # digital root result
    pushl -4(%ebp)           # original number
    pushl $.LC2              # push format string
    call  printf             # print result
    addl  $12, %esp          # clean up stack without pop since value should be discarded
    
    movl $0, %eax            # return 0
    leave
    ret

# Function: dr
# Purpose: Calculate digital root recursively
# Parameters: n (pushed on stack)
# Returns: digital root in %eax
dr:
    pushl %ebp               # set up stack frame
    movl  %esp, %ebp         # set up stack frame
    subl  $4, %esp           # space for sum
    
    movl  8(%ebp), %eax      # get n
    cmpl  $10, %eax          # compare n with 10
    jl    .base_case         # if n < 10, return n
    
    # recursive case: compute sum of digits
    movl  $0, -4(%ebp)       # sum = 0
    
.sum_loop:
    cmpl  $0, %eax           # while n > 0
    je    .sum_done          # if n == 0, done

    # compute n % 10 and n / 10 without divl
    movl  %eax, %ecx         # save n in ecx
    movl  $0, %ebx           # quotient = 0
    
.div_loop:
    cmpl  $10, %ecx          # compare n with 10
    jl    .div_done          # if n < 10, done dividing
    subl  $10, %ecx          # n = n - 10
    incl  %ebx               # quotient++
    jmp   .div_loop          # repeat
    
.div_done:
    # ecx is n % 10 (remainder)
    # ebx is n / 10 (quotient)
    addl  %ecx, -4(%ebp)     # sum += digit (remainder)
    movl  %ebx, %eax         # n = quotient
    jmp   .sum_loop          # repeat
    
.sum_done:
    pushl -4(%ebp)           # recursive call with sum
    call  dr                 # call dr(sum)
    addl  $4, %esp           # clean up stack
    jmp   .done              # return result
    
.base_case:
    # base case: n < 10
    # return n
    # needed to prevent errors
    jmp   .done 

.done:
    leave                   # restore stack frame
    ret
