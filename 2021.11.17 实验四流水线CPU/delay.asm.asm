.text
lw	$t1, 1
lw	$t2, 2
beq	$t1, $t2, jump
jump:
	add $t3, $t1, $t2