.text
main:
	li	$v0,	5	#	input an integer
	syscall
	move	$t1,	$v0	#	t1 = int a
	li	$v0,	5	#	input an integer
	syscall	
	move	$t2,	$v0	#	t2 = int b, a<b
	
Loop:
	div	$t2,	$t1
	mfhi	$t4		#	get number of %
	beq	$t4,	$0,	Exit
	move	$t2,	$t1
	move	$t1,	$t4
	j	Loop

Exit:
	move 	$a0,	$t1
	li	$v0,	1
	syscall
	li	$v0,	10
	syscall
