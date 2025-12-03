# File: bubblesort.s
# Purpose: sort an array of integers using bubble sort
# Author: Sean Balbale
# Date: 12/03/2025

.section .text
.global bubble_sort

# Function: bubble_sort
# Purpose: Sort an array of integers using bubble sort
# Arguments: arr - pointer to the array
#            n   - number of elements in the array
# Returns: void
bubble_sort:
    pushl %ebp              # save previous frame pointer
    movl %esp, %ebp         # set new stack frame
    pushl %ebx              # save callee-saved registers
    pushl %esi
    pushl %edi

    movl 8(%ebp), %esi      # arr pointer
    movl 12(%ebp), %ecx     # n (number of elements)
    
    cmpl $1, %ecx           # if n <= 1, done
    jle .done

    decl %ecx               # n - 1 (outer loop limit)
    movl $0, %ebx           # i = 0

.outer_loop:
    cmpl %ecx, %ebx         # if i >= n - 1, done
    jge .done

    movl $0, %edx           # j = 0
    
    # Calculate inner loop limit: n - 1 - i
    movl %ecx, %edi         # limit = n - 1
    subl %ebx, %edi         # limit = n - 1 - i

.inner_loop:
    cmpl %edi, %edx         # if j >= limit, next outer
    jge .next_outer

    # Compare arr[j] and arr[j+1]
    movl (%esi, %edx, 4), %eax      # arr[j]
    
    # Compare arr[j+1] with arr[j]
    # Note: cmpl A, B computes B - A
    cmpl %eax, 4(%esi, %edx, 4)     
    jge .no_swap                    # if arr[j+1] >= arr[j], no swap

    # Swap
    # arr[j] is in eax
    pushl %edi                      # save limit (edi)
    movl 4(%esi, %edx, 4), %edi     # arr[j+1] -> edi
    movl %edi, (%esi, %edx, 4)      # arr[j] = arr[j+1]
    movl %eax, 4(%esi, %edx, 4)     # arr[j+1] = old arr[j] (eax)
    popl %edi                       # restore limit

.no_swap:
    incl %edx               # j++
    jmp .inner_loop

.next_outer:
    incl %ebx               # i++
    jmp .outer_loop

.done:
    popl %edi
    popl %esi
    popl %ebx               # restore callee-saved registers
    leave                   # restore previous frame pointer
    ret                     # return from function
