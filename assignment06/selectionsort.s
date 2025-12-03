# File: selectionsort.s
# Purpose: sort an array of integers using selection sort
# Author: Sean Balbale
# Date: 12/03/2025

.section .text
.global selection_sort

# Function: selection_sort
# Purpose: Sort an array of integers using selection sort
# Arguments: arr - pointer to the array
#            n   - number of elements in the array
# Returns: void
selection_sort:
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

    movl %ebx, %edi         # min_idx = i
    movl %ebx, %edx         # j = i
    incl %edx               # j = i + 1
    
    # Inner loop condition: j <= n - 1 (which is in ecx)

.inner_loop:
    cmpl %ecx, %edx         # if j > n - 1
    jg .swap_min

    # Compare arr[j] < arr[min_idx]
    movl (%esi, %edx, 4), %eax      # arr[j]
    
    # We need to compare with arr[min_idx]. 
    # We can use a temp register or stack.
    pushl %ecx                      # save n-1
    movl (%esi, %edi, 4), %ecx      # arr[min_idx]
    cmpl %ecx, %eax                 # compare arr[j] with arr[min_idx]
    popl %ecx                       # restore n-1
    
    jge .next_inner                 # if arr[j] >= arr[min_idx], skip

    movl %edx, %edi                 # min_idx = j

.next_inner:
    incl %edx               # j++
    jmp .inner_loop

.swap_min:
    # Swap arr[i] and arr[min_idx]
    cmpl %ebx, %edi                 # if min_idx == i, no swap needed
    je .next_outer

    movl (%esi, %ebx, 4), %eax      # arr[i]
    movl (%esi, %edi, 4), %edx      # arr[min_idx] (reuse edx as scratch)
    movl %edx, (%esi, %ebx, 4)      # arr[i] = arr[min_idx]
    movl %eax, (%esi, %edi, 4)      # arr[min_idx] = old arr[i]

.next_outer:
    incl %ebx               # i++
    jmp .outer_loop

.done:
    popl %edi
    popl %esi
    popl %ebx               # restore callee-saved registers
    leave                   # restore previous frame pointer
    ret                     # return from function
