#MARS
ori $s0, $s0, 0
ori $s1, $s1, 3
ori $s7, $s7, 3
sw $s1, 0($s0)
lw $s2, 0($s0)
beq $s7, $s2, End
ori $s3, $s2, 4
End:
sw $s3, 4($s0)
