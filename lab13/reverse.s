# File: reverse.s
# Purpose: reverse an array of integers in place
# Author: Sean Balbale
# Date: 11/11/2025


.section .text
.global reverse

# Function: reverse
# Purpose: reverse an array of integers inplace
# Arguments: arr - pointer to the array
#            n   - number of elements in the array
# Returns: void
reverse:
    pushl %ebp              # save previous frame pointer
    movl %esp, %ebp         # set new stack frame
    pushl %ebx              # save callee-saved registers

    movl 8(%ebp), %esi      # arr pointer
    movl 12(%ebp), %edi     # n (number of elements)
    movl $0, %edx           # put i in edx
    decl %edi               # right index
    movl %eax,%esi

.loop:
    cmpl %edx,%edi              # compare l and r index
    jle .done                    # jump if over half

    movl (%esi,%edx,4),%ecx         # temp = arr[i]
    movl (%esi,%edi,4), %eax        # put last in eax
    movl %eax,(%esi,%edx,4)                  # put eax in esi
    movl %ecx,(%esi,%edi,4)         # put temp in last

    incl %edx                        # i++
    decl %edi
    jmp .loop


.done:
    leave                   # restore previous frame pointer
    ret                     # return from function






