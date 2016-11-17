/*Jarod England operations.s C335 HW5*/
	.text
	.syntax unified
	.global alloc
	.type alloc, %function
alloc:
	nop
	push {r3,1r}
	b1 malloc
	pop {r1,pc}

	.global dealloc
	.type deallocc, %function
dealloc:
	nop
	push {r3, 1r}
	b1 free
	pop {r3, pc}
	.end

//	.global add
//	.type add, %function
//add:
//	nop
//???
