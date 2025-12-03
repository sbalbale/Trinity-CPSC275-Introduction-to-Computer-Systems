# File: echoline.s
# Purpose: Read and echo a line of input from the user.
#           The buffer for the input line must be allocated on the stack in main.
# Author: Sean Balbale
# Date: 11/4/2025


.LC0:
    .string "%c"            # format string for reading a single character
    .text

.LC1:
    .string "%s\n"          # format string for printing the echoed line
    .text

.globl main
main:
    subl  $200, %esp         # allocate 200 bytes for input buffer on stack

    movl  %esp, %eax         # get address of buffer (now at %esp)
    pushl %eax               # push buffer address as parameter to getline
    call  getline            # call getline to read input
    addl  $4, %esp           # clean up stack

    movl  %esp, %eax         # get address of buffer (at %esp again)
    pushl %eax               # push buffer address for printing
    pushl $.LC1              # format string for printing (%s)
    call  printf             # print the echoed line
    addl  $8, %esp           # clean up stack

    addl  $200, %esp         # deallocate buffer
    movl $0, %eax            # return 0
    ret

# Function: getline
# Purpose: Read a line from standard input into the buffer pointed by parameter
# Parameter: char *buf (at 8(%ebp))
# Returns: void
.global getline
getline:
    pushl %ebp               # set up stack frame
    movl  %esp, %ebp         # set up stack frame
    subl  $8, %esp           # allocate space for local variable (char)
    pushl %ebx               # save %ebx (callee-saved)

    movl  8(%ebp), %ebx      # get buffer address from parameter into %ebx
.read_loop:
    leal  -8(%ebp), %eax     # get address of local char variable
    pushl %eax               # push address for scanf to store character
    pushl $.LC0              # push format string for single character
    call  scanf              # read a character
    addl  $8, %esp           # clean up stack
    cmpl  $1, %eax           # check if scanf succeeded (returns 1 on success)
    jne   .done              # if scanf failed done
    movb  -8(%ebp), %al      # load the character into %al
    cmpb  $'\n', %al         # check if character is newline
    movb  %al, (%ebx)        # store character in buffer
    je    .done              # if newline, finish reading
    addl  $1, %ebx           # move buffer pointer to next character
    jmp   .read_loop

.done:
    movb  $0, (%ebx)         # null-terminate the string
    popl  %ebx               # restore %ebx
    leave                    # restore previous frame pointer
    ret                      # return from function


