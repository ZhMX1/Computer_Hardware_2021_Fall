.data
	string:	.space	4040
.text
main:
	li	$v0,	5
	syscall
	move	$s0,	$v0	#n = string length	$s0
	li	$v0,	5
	syscall
	move	$s1,	$v0	# x = start	$s1
	li	$v0,	5
	syscall
	move	$s2,	$v0	# y = end	$s2
	la	$a0,	string 
        li	$a1,	4040
        li	$v0,	8
        syscall
        move	$t0,	$a0	# ÊäÈë×Ö·û´®	$t0 = string
        
ForLoop:
	bge	$s1,	$s2,	Exit
	la	$a0,	string
	move	$a1,	$s1
	move	$a2,	$s2	# ´«²Î
	jal	swap
	addi	$s1,	$s1,	1
	subi	$s2,	$s2,	1
	j	ForLoop
swap:
	sll	$t1,	$a1,	0	# $t1 = start address = x*4
	add	$t1,	$t1,	$a0	# $t1 = string + x*4
	sll	$t2,	$a2,	0	# $t2 = end address = y*4
	add	$t2,	$t2,	$a0	# $t2 = string + y*4
 	lb	$t3,	0($t1)		# $t3 = string[x]
	lb 	$t4,	0($t2)		# $t4 = string[y]
	sb	$t4,	0($t1)		# string[x] = $t4
	sb	$t3,	0($t2)		# string[y] = $t3
	jr	$ra           # return    
Exit:
	la	$a0,	string 
	li	$v0,	4
	syscall
	li	$v0,	10
	syscall
