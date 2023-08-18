# 快排MIPS

c语言版

```c
int getStandard(int array[], int i, int j) {
    // 基准数据
    int key = array[i];
    while (i < j) {
        // 因为默认基准是从左边开始，所以从右边开始比较
        // 当队尾的元素大于等于基准数据 时,就一直向前挪动 j 指针
        while (i < j && array[j] >= key) {
            j--;
        }
        // 当找到比 array[i] 小的时，就把后面的值 array[j] 赋给它
        if (i < j) {
            array[i] = array[j];
        }
        // 当队首元素小于等于基准数据 时,就一直向后挪动 i 指针
        while (i < j && array[i] <= key) {
            i++;
        }
        // 当找到比 array[j] 大的时，就把前面的值 array[i] 赋给它
        if (i < j) {
            array[j] = array[i];
        }
    }
    // 跳出循环时 i 和 j 相等,此时的 i 或 j 就是 key 的正确索引位置
    // 把基准数据赋给正确位置
    array[i] = key;
    return i;
}

void QuickSort(int array[], int low, int high) {
    // 开始默认基准为 low
    if (low < high) {
        // 分段位置下标
        int standard = getStandard(array, low, high);
        // 递归调用排序
        // 左边排序
        QuickSort(array, low, standard - 1);
        // 右边排序
        QuickSort(array, standard + 1, high);
    }
}
```

MIPS汇编翻译版

```mips
.data
 array:.space 4000
 str_comma:.asciiz","
 str_left_kuo:.asciiz"["
 str_right_kuo:.asciiz"]"
.text
  li $v0,5
  syscall
  move $s0,$v0 #s0为个数
  li $t0,0
  li $s2,0 #low
  addi $s3,$s0,-1 #high
  sw $s2,-4($sp)
  sw $s3,-8($sp)
input:
  beq $t0,$s0,end_input
  sll $t1,$t0,2
  li $v0,5
  syscall
  sw $v0,array($t1)
  addi $t0,$t0,1
  j input
end_input:
  li $t0,0
  li $t1,0 #把寄存器清零先
  jal QuickSort#之后就是纯输出
  li $t0,0
  la $a0,str_left_kuo
  li $v0,4
  syscall
  addi $s0,$s0,-1 #为了让逗号能不多
  out:
   beq $t0,$s0,finish
   sll $t1,$t0,2
   lw $t3,array($t1)
   move $a0,$t3
   li $v0,1
   syscall
   la $a0,str_comma
   li $v0,4
   syscall
   addi $t0,$t0,1
   j out
 finish: 
   sll $t1,$t0,2
   lw $t3,array($t1)
   move $a0,$t3
   li $v0,1
   syscall
  la $a0,str_right_kuo
  li $v0,4
  syscall
  li $v0,10
  syscall
  
 getStandard:
   lw $t0,8($sp) #j
   lw $t1,12($sp) #i
   sll $t2,$t1,2
   lw $s1,array($t2) #key,$t3=array[i]
   li $t7,2
   loop1:
      bge $t1,$t0,end_loop1
        loop2:
           slt $t4,$t1,$t0
           sll $t2,$t0,2
           lw $t5,array($t2) #$t5=array[j]
           sle $t6,$s1,$t5
           addu $t4,$t4,$t6
           bne $t4,$t7,end_loop2
           addi $t0,$t0,-1
           j loop2
      end_loop2:
         bge $t1,$t0,loop3
         sll $t2,$t0,2
         lw $t5,array($t2) #$t5=array[j]
         sll $t2,$t1,2
         sw $t5,array($t2)
     loop3:
         slt $t4,$t1,$t0
         sll $t2,$t1,2
         lw $t3,array($t2) #$t3=array[i]
         sle $t6,$t3,$s1
         addu $t4,$t4,$t6
         bne $t4,$t7,end_loop3
         addi $t1,$t1,1
         j loop3
     end_loop3:
         bge $t1,$t0,again
         sll $t2,$t1,2
         lw $t3,array($t2) #$t3=array[i]
         sll $t2,$t0,2
         sw $t3,array($t2)
      again:
         j loop1
    end_loop1:
       sll $t2,$t1,2
       sw $s1,array($t2)
       sw $t1,4($sp) #每次$sp都对准$ra
       jr $ra
    
QuickSort:
    addi $sp,$sp,-16
    sw $ra,0($sp)
    lw $s2,12($sp) #low
    lw $s3,8($sp) #high
    bge $s2,$s3,end
    #sw $s2,-4($sp)
    #sw $s3,-8($sp)
    jal getStandard
    lw $s4,4($sp) #standard
    lw $s2,12($sp) #low
    lw $s3,8($sp) #high
    sw $s2,-4($sp)
    addi $s5,$s4,-1
    sw $s5,-8($sp)
    jal QuickSort
    
    lw $s2,12($sp) #low 这里要重新取一下low和high
    lw $s3,8($sp) #high
    lw $s4,4($sp)
    sw $s3,-8($sp)
    addi $s5,$s4,1
    sw $s5,-4($sp)
    jal QuickSort
   end:
     lw $ra,0($sp)
     addi $sp,$sp,16
     jr $ra
```

## 汉诺塔

```
#include<stdio.h>
void hanoi(char A,char B,char C,int n)
{
if(n==1)
{
  printf("%c to %c\n",A,C);
  return ;
}
else
{
  hanoi(A,C,B, n-1);
  printf("%c to %c\n",A,C);
  hanoi(B,A,C,n-1);
}
}
int main()
{
int n;
scanf("%d",&n);
hanoi('A','B','C',n);
}
```

MIPS

```mips
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

```

