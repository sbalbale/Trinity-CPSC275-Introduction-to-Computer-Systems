# File: mywc.s
# Purpose: Read a line from standard input and print the number of words
#          and characters in the input line.
# Author: Sean Balbale
# Date: 11/4/2025

.section .data
.LC0:
    .string "%c"
    .text

.LC1:
    .string "%d %d\n"
    .text

.section .text
.globl main
main:
    subl  $200, %esp         # allocate 200 bytes for input buffer on stack

    movl  %esp, %eax         # get address of buffer
    pushl %eax               # push buffer address as parameter to getline
    call  getline            # call getline to read input
    addl  $4, %esp           # clean up stack

    movl  %esp, %eax         # get address of buffer
    pushl %eax               # push buffer address for countchars
    call  countchars         # count characters
    addl  $4, %esp           # clean up stack
    pushl %eax               # save char count on stack

    movl  %esp, %eax         # get address of buffer (skip saved count)
    addl  $4, %eax           # adjust for saved count
    pushl %eax               # push buffer address for countwords
    call  countwords         # count words
    addl  $4, %esp           # clean up stack
    
    popl  %edx               # restore char count to %edx
    pushl %edx               # push char count for printf
    pushl %eax               # push word count for printf
    pushl $.LC1              # push format string
    call  printf             # print results
    addl  $12, %esp          # clean up stack

    addl  $200, %esp         # deallocate buffer
    movl  $0, %eax           # return 0
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
    jne   .done              # if scanf failed, we're done
    movb  -8(%ebp), %al      # load the character into %al
    cmpb  $'\n', %al         # check if character is newline
    movb  %al, (%ebx)        # store character in buffer
    je    .done              # if newline, finish reading
    addl  $1, %ebx           # move buffer pointer to next character
    jmp   .read_loop

.done:
    movb  $0, (%ebx)         # null-terminate the string
    popl  %ebx               # restore %ebx
    leave
    ret

# Function: countwords
# Purpose: Count the number of words in a line (words separated by single space)
# Parameter: char *line (at 8(%ebp))
# Returns: int (number of words)
.global countwords
countwords:
    pushl %ebp               # set up stack frame
    movl  %esp, %ebp         # set up stack frame
    pushl %ebx               # save %ebx (callee-saved)
    
    movl  8(%ebp), %ebx      # get line address from parameter
    movl  $0, %ecx           # word count = 0
    movl  $0, %edx           # in_word flag = 0 (false)

.count_loop:
    movzbl (%ebx), %eax      # load character (zero-extend byte to dword)
    testb %al, %al           # check if null terminator
    je    .count_done        # if null, we're done
    
    cmpb  $' ', %al          # check if space
    je    .is_space          # if space, handle it
    
    # Not a space - we're in a word
    cmpl  $0, %edx           # check if already in a word
    jne   .next_char         # if already in word, continue
    incl  %ecx               # increment word count (new word)
    movl  $1, %edx           # set in_word flag = 1 (true)
    jmp   .next_char

.is_space:
    movl  $0, %edx           # clear in_word flag (not in word)

.next_char:
    incl  %ebx               # move to next character
    jmp   .count_loop

.count_done:
    movl  %ecx, %eax         # move word count to return register
    popl  %ebx               # restore %ebx
    leave
    ret

# Function: countchars
# Purpose: Count the number of characters in a line (excluding null terminator)
# Parameter: char *line (at 8(%ebp))
# Returns: int (number of characters)
.global countchars
countchars:
    pushl %ebp               # set up stack frame
    movl  %esp, %ebp         # set up stack frame
    pushl %ebx               # save %ebx (callee-saved)
    
    movl  8(%ebp), %ebx      # get line address from parameter
    movl  $0, %eax           # character count = 0

.char_loop:
    movzbl (%ebx), %ecx      # load character (zero-extend byte to dword)
    testb %cl, %cl           # check if null terminator
    je    .char_done         # if null, we're done
    
    incl  %eax               # increment character count
    incl  %ebx               # move to next character
    jmp   .char_loop

.char_done:
    popl  %ebx               # restore %ebx
    leave
    ret
