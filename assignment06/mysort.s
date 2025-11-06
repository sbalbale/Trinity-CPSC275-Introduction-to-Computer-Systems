# File: mysort.s
# Purpose: sort an array of integers using insertion sort
# Author: Sean Balbale
# Date: 11/11/2025


.section .text
.global insertion_sort

# Function: insertion_sort
# Purpose: Sort an array of integers using insertion sort
# Arguments: arr - pointer to the array
#            n   - number of elements in the array
# Returns: void
insertion_sort:
    pushl %ebp              # save previous frame pointer
    movl %esp, %ebp         # set new stack frame
    pushl %ebx              # save callee-saved registers

    movl 8(%ebp), %esi      # arr pointer
    movl 12(%ebp), %ecx     # n (number of elements)
    decl %ecx               # n - 1
    jle .done               # if n <= 1, done
    movl $1, %ebx           # i = 1
.outer_loop:
    movl (%esi, %ebx, 4), %edx  # key = arr[i]
    movl %ebx, %eax          # j = i
.inner_loop:
    decl %eax                # j = j - 1
    cmpl $0, %eax            # while j >= 0
    jl .insert_key           # if j < 0, insert key
    movl (%esi, %eax, 4), %edi # arr[j]
    cmpl %edx, %edi         # compare arr[j] and key
    jle .insert_key          # if arr[j] <= key, insert key
    movl %edi, 4(%esi, %eax, 4) # arr[j + 1] = arr[j]
    jmp .inner_loop          # repeat inner loop
.insert_key:
    movl %edx, 4(%esi, %eax, 4) # arr[j + 1] = key
    incl %ebx                # i = i + 1
    cmpl %ecx, %ebx         # compare i and n - 1
    jle .outer_loop          # repeat outer loop
.done:
    popl %ebx               # restore callee-saved registers
    leave
    ret                     # return from function
