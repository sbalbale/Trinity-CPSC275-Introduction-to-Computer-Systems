.LCO:
	.string	"x = %d y = %d\n"
	

	.globl	main
main:
	pushl	%ebp			# save frame pointer
	movl	%esp, %ebp		# adjust stack pointer

	movl	$10, %eax		# put 10 at eax
	movl	$20, %ebx		# put 20 at ebx
	pushl	%ebx			# push ebx to stack
	pushl	%eax			# push eax to stack
	pushl	$.LCO			# push .LCO code to stack
	call	printf			# print stack to terminal

	leave					# restore current activation
	ret						# return to caller
	