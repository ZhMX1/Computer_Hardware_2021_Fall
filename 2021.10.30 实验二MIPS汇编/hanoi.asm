.data
	a:.asciiz"A"	# 1->A
	b:.asciiz"B"	# 2->B
	c:.asciiz"C"	# 3->C
	mid:.asciiz"->"
	end:.asciiz "\n"
.text
main:
	li	$v0,	5
	syscall
	move	$s0,	$v0	# $s0 = n
	li	$s1,	65
	li	$s2,	66
	li	$s3,	67	#ascii，输出对应的字符
	sw	$s0,	-4($sp)
	sw	$s1,	-8($sp)
	sw	$s2,	-12($sp)
	sw	$s3,	-16($sp)
	jal	hanoi
	li	$v0,	10
	syscall
	
	hanoi:
		subi	$sp,	$sp,	20
		sw	$ra,	0($sp)
		lw	$s0,	16($sp)	# n
		li	$t0,	1
		beq	$s0,	$t0,	endPrint
		
		lw	$s0,	16($sp)
		subi	$s0,	$s0,	1	# n-1
		lw	$s1,	12($sp)	# A
		lw	$s2,	4($sp)	# C
		lw	$s3,	8($sp)	# B
		sw	$s0,	-4($sp)
		sw	$s1,	-8($sp)
		sw	$s2,	-12($sp)
		sw	$s3,	-16($sp)
		jal	hanoi
		
		lw	$a0,	12($sp)
		li	$v0,	11
		syscall
		la	$a0,	mid
		li	$v0,	4
		syscall
		lw	$a0,	4($sp)
		li	$v0,	11
		syscall
		la	$a0,	end
		li	$v0,	4
		syscall
		
		lw	$s0,	16($sp)
		subi	$s0,	$s0,	1	# n-1
		lw	$s1,	8($sp)
		lw	$s2,	12($sp)
		lw	$s3,	4($sp)
		sw	$s0,	-4($sp)
		sw	$s1,	-8($sp)
		sw	$s2,	-12($sp)
		sw	$s3,	-16($sp)
		jal	hanoi
		j	return
endPrint:
	lw	$a0,	12($sp)
	li	$v0,	11
	syscall
	la	$a0,	mid
	li	$v0,	4
	syscall
	lw	$a0,	4($sp)
	li	$v0,	11
	syscall
	la	$a0,	end
	li	$v0,	4
	syscall
	j	return
return:	
	lw	$ra,	0($sp)
	addi	$sp,	$sp,	20
	jr	$ra
