.text
lui $s1, 1
lui $s2, 2
lui $s3, 3
lui $s4, 4
ori $s1, $s1, 1
ori $s2, $s2, 2
ori $s3, $s3, 3
ori $s4, $s4, 4
sw $s1,0($0)
sw $s2,4($0)
sw $s3,8($0)
sw $s4,12($0)
lw $s5, 0($0)
lw $s6,4($0)
lw $s7,8($0)
lw $t8,12($0)
