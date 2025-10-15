.LC0:
	.string	"x = %d y = %d\n"

	.globl	main
main:
	pushl	%ebp
	movl	%esp,%ebp

	movl	$10,%eax
	movl	$20,%ebx
	pushl	%ebx
	pushl	%eax
	pushl	$.LC0
	call	printf

	leave
	ret
