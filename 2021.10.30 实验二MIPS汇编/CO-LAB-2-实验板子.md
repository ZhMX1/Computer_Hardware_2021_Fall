# 冒泡排序

```
int main()
{
	int a[N] = { 3,4,6,7,1,2,8,9,5,0 };
	int temp, i, j;

	for (i = 0; i < N-1; i++)    //进行9次比较
		for (j = 0; j < N - i - 1; j++)   //在每次中进行10-i-1次比较
			if (a[j] > a[j + 1])      //按升序排序，降序用<
			{
				temp = a[j + 1];      //交换相邻的两个元素
				a[j + 1] = a[j];
				a[j] = temp;
			}
	printf("排序后结果为:\n");  //输出排序后的结果
	for (i = 0; i < N; i++)
		printf("%d ", a[i]);
}
```

MIPS版

```mips
.text                           #代码段
main:                           #主程序
      li $v0, 9                 #9号syscall,请求内存空间，也可以先开一个.space
      li $a0, 40                #申请40byte,4word的空间大小
      syscall                   #系统调用

      add  $s1, $v0, $zero       #加载基准内存地址，也可以move	$s1,	$v0
      add  $s0, $zero, $zero     # $s0 就是 int i
read: 
                                #读数开始
      li $v0, 5                 #从键盘读整数
      syscall                   #系统调用,读到$v0中
      sll $t0, $s0, 2           #偏移量 $t0 =i*4
      add $t1, $t0, $s1         #$t1 实际地址 = 偏移量$t0 + 基准地址
      sw $v0, 0($t1)            #写入到实际地址 $t1
                                #读数结束的判断
      addi $s0, $s0, 1          #i = i + 1
      slti $t0, $s0, 10         #若i<n（找个寄存器存一下（set less than
      bne  $t0, $zero, read     #则继续循环

sort:
      add  $s0, $zero, $zero     # $s0 就是 int i = 0 清零

oLop:
                                #外层循环开始
      addi $s2, $zero, 9        #$s2 就是 int j = 9
	iLop:                           #内层循环开始      
          sll $t0, $s2, 2           #偏移量j*4
          add $t1, $s1, $t0         #A[j]的实际内存地址
          addi $t2, $t1, -4         #A[j-1]的实际内存地址
          lw $t3, 0($t1)            #$t3=A[j]的值
          lw $t4, 0($t2)            #$t4=A[j-1]的值

          slt $t5, $t4, $t3         #若A[j-1] < A[j]
          beq $t5, $zero, afterSwap #为真交换，否则跳过

#swap
          lw $t6, 0($t1)            # tmp=A[j]
          sw $t4, 0($t1)            # A[j] = A[j-1]
          sw $t6, 0($t2)            # A[j-1]=tmp
afterSwap:
          addi $s2, $s2, -1          # j = j - 1
          slt $t0, $s0, $s2         #若i < j
          bne $t0, $zero, iLop      #继续内层循环
      addi $s0, $s0, 1          # i = i + 1
      slti $t0, $s0, 9          #若i<9
      bne  $t0, $zero, oLop     #则继续外层循环     
                                #外层循环结束

#print 
                        #基准地址是$s1
      add  $s0, $zero, $zero    # $s0 就是 int i
print:                          # 输出开始
      li $v0, 1                 #置输出int函数ID
      sll $t0, $s0, 2           #偏移量 $t0 =i*4
      add $t1, $t0, $s1         #$t1 实际地址 = 偏移量$t0 + 基准地址
      lw $a0, 0($t1)      #写入输出参数到 $a0
      syscall                   #系统调用,输出排序后整数

      li $v0, 11                #置输出char函数ID
      add $a0, $zero, 32       #置输出参数： 空格 
      syscall                   #输出空格

      addi $s0, $s0, 1          #i = i + 1
      slti $t0, $s0, 10         #若i<10
      bne  $t0, $zero, print    #则继续循环

      li $v0, 10                #停
      syscall                   #机         
```



# 约瑟夫环

```c
int f(int n, int m){
    if(n == 1)   return n;
    return (f(n - 1, m) + m - 1) % n + 1;
}
```



```c
#define length 100
void Joseph(int n,int m)
{
	int alive=n;
	int num=0;
	int index=0;//下标，==编号-1 
	
	int cunzai[length];
	for(int i=0;i<length;i++)
		cunzai[i]=1;
	while(alive>0)
	{
		num+=cunzai[index];//计数，每轮到一个人报数即num+1
		if(num==m)//满足淘汰条件 
		{
			print("%d ",index+1)；
			cunzai[index]=0;
			alive--;
			num=0; 
		} 
		index=(index+1)%n;//保证每次的下标都在0-n-1的范围内 
	} 
	print("\n"); 
}
int main()
{
	scanf("%d",&n);
	scanf("%d",&m);
	Joseph(n,m);
	return 0;
}
```

MIPS版

```mips
.macro address(%des,%column)	#初始化cunzai数组为1
	li %des,0
	addu %des,%des,%column
	sll %des,%des,2
.end_macro
.data
cunzai:.word 0:47
space:.asciiz" "
enter:.asciiz"\n"
.text
	li $v0,5
	syscall 
	move $s0,$v0 #输入n
	li $v0,5
	syscall
	move $s1,$v0 #输入m
	
	li $t0,0
	li $t2,1
initial:
	address($t1,$t0)
	sw $t2,cunzai($t1)
	addi $t0,$t0,1
	bne $t0,47,initial
	
Joseph:
	move $t0,$s0 #alive，当前幸存人数，初始值为n
	li $t1,0 #计数num，当$t1==$s1-1时淘汰这个人
	li $t2,0 #下标index，为当前的总人数-1
while_begin:
		blez $t0,while_end
		address($t3,$t2)
		lw $t4,cunzai($t3) #cunzai[index]
		addu $t1,$t1,$t4 #number += 1- circle[index];
		bne $t1,$s1,if_end #还未数到m，继续
			addi $a0,$t2,1
			li $v0,1
			syscall #print(index+1)
			la $a0,enter
			li $v0,4
			syscall #print(" ")
			li $t4,0
			sw $t4,cunzai($t3) #circle[index]=0;
			subi $t0,$t0,1 #alive--;
			li $t1,0 #num=0;
		if_end:
		addi $t4,$t2,1
		divu $t4,$s0
		mfhi $t2 #index = (index +1) % count;
		j while_begin
while_end:
	la $a0,enter
	li $v0,4
	syscall
	li $v0,10
	syscall
```

# 斐波那契数列

```c
int Fibon1(int n){
    if (n == 1 || n == 2){
        return 1;
    } else{
        return Fibon1(n - 1) + Fibon1(n - 2);
    }
}
int main(){
    int n = 0;
    int ret = 0;
    scanf("%d", &n);
    ret = Fibon1(n);
    printf("ret=%d", ret);
    return 0;
}
```

递归实现

```mips
.data
array: 	.space 1024
hello:	.asciiz "Input a number: \n"
bye:	.asciiz "\nSayonala\n"
.text	
	li $v0,4
	la $a0,hello
	syscall 
	li $v0,5
	syscall
	move $t2,$v0  
	move $t3,$t2
	li $t0,1		#set array(0) and array(1) = 1
	la $t1,array
	sw $t0,($t1)
	sw $t0,4($t1)
	blt $t2,3,print	#if n<3 ,just print,needless of recursion
	
Fib:	
	addi $sp,$sp,-8
	sw $ra,0($sp)
	sw $t2,4($sp)
	#base case
	beq $t2,2,re	#when $t2=2 ,we're gonna return
	addi $t2,$t2,-1
	jal Fib
	# Fib(n)=Fib(n-1)+Fib(n-2)
	move $t4,$0
	lw $a0,($t1)
	lw $a1,4($t1)
	add $t4,$a1,$a0
	sw $t4,8($t1)
	
	addi $t1,$t1,4
	beq $t2,$t3,print
	
re:	lw $ra,0($sp)
	addi $t2,$t2,1
	addi $sp,$sp,8
	jr $ra		#we're gonna do from line 39 

print:	la $t1,array
	li $t3,1
loop1:	lw $a0,($t1)
	li $v0,1
	syscall
	
	li $v0,11
	li $a0,' '
	syscall
	
	beq $t3,$t2,end
	addi $t3,$t3,1
	addi $t1,$t1,4
	b loop1

end:	li $v0,4  
	la $a0,bye
	syscall
	li $v0,10
	syscall
```

```mips
li $v0,5
syscall
move $a0,$v0
jal f
move $a0,$v0
li $v0,1
syscall
li $v0,10
syscall
f:	li $t2,2
	bgt $a0,$t2,recur
	li $v0,1
	jr $ra
recur:	addi $sp,$sp,-8
	sw $a0,0($sp)
	sw $ra,4($sp)
	#after store,find f(n-1)
	addi $a0,$a0,-1
	jal f
	#after f(n-1) was found,then lw and up_sp
	lw $ra,4($sp)
	lw $a0,0($sp)
	addi $sp,$sp,8
	#set $s1 as $v0(f(n-1))
	move $s1,$v0
	#find f(n-2),another recur
	addi $sp,$sp,-12
	sw $s1,0($sp)
	sw $a0,4($sp)
	sw $ra,8($sp)
	addi $a0,$a0,-2
	jal f
	lw $ra,8($sp)
	lw $a0,4($sp)
	lw $s1,0($sp)
	addi $sp,$sp,12
	add $v0,$v0,$s1
	jr $ra
```

循环实现

```mips
       .text
        .globl main
main:
        li    $v0,   4         #打印字符串
        la    $a0,   msg1
        syscall

        li    $v0,   5          #输入初始值
        syscall
        move    $a0,   $v0

        move   $v0,   $a0
        blt    $a0,   2,   done
        li     $t0,   0
        li     $v0,   1
      
fib:    add    $t1,   $t0,  $v0
        move   $t0,   $v0        #更新次上一个数值，即F(n-2)
        move   $v0,   $t1        #更新上一个斐波那契数值F(n-1)
        sub    $a0,   $a0,  1
        bgt    $a0,   1,    fib
done:   sw     $v0,   result

        move    $a0,   $v0    #测试结果是否正确
        li     $v0,    1
        syscall

############### data segment   #####
       .data
result: .word  0x00000000
msg1:   .asciiz  "/ninput initial N:/n"

```

# 汉诺塔补充

```mips
.macro swap(%a,%b)
	move $t0,%a
	move %a,%b
	move %b,$t0
.end_macro
.macro save(%a) #%a里是要存储的值
	subi $sp,$sp,4
	sw %a,0($sp)
.end_macro
.macro load(%a)#%a里是要得到的值
	lw %a,0($sp)
	addi $sp,$sp,4
.end_macro
.data
str_connect:.asciiz "-->"
str_enter:.asciiz "\n"
str_from:.asciiz "A"
str_tmp:.asciiz "B"
str_to:.asciiz "C"
.text
	li $v0,5
	syscall
	move $s0,$v0
	
	la $a0,str_from
	la $a1,str_tmp
	la $a2,str_to
	move $a3,$s0
	jal f_move
	
	li $v0,10
	syscall
	
f_move:
	beq $a3,1,equal
	save($ra)
	save($a0)
	save($a1)
	save($a2)
	save($a3)
	subi $a3,$a3,1
	swap($a1,$a2)
	jal f_move
	load($a3)
	load($a2)
	load($a1)
	load($a0)
	load($ra)
	
	save($ra)
	save($a0)
	save($a1)
	save($a2)
	save($a3)
	li $a3,1
	jal f_move
	load($a3)
	load($a2)
	load($a1)
	load($a0)
	load($ra)
	
	save($ra)
	save($a0)
	save($a1)
	save($a2)
	save($a3)
	subi $a3,$a3,1
	swap($a0,$a1)
	jal f_move
	load($a3)
	load($a2)
	load($a1)
	load($a0)
	load($ra)
	
	jr  $ra
equal:
	move $a0,$a0
	li $v0,4
	syscall
	la $a0,str_connect
	li $v0,4
	syscall
	move $a0,$a2
	li $v0,4
	syscall
	la $a0,str_enter
	li $v0,4
	syscall
	jr $ra
```



# 回文串判断

```mips
.macro get_address(%des,%column)
	addu %des,%des,%column
	sll %des,%des,2
.end_macro 
.data
string:.word 20

#输入：
li $v0,5
	syscall
	move $s0,$v0 #n
	
	li $t0,0
read:
	li $v0,12
	syscall
	sll $t1,$t0,2
	sw $v0,string($t1)
	addi $t0,$t0,1
	bne $t0,$s0,read
	
#判断：
	li $t0,0 #a
	subi $t1,$s0,1 #b
check:
	li $t2,0
	li $t3,0
	get_address($t2,$t0)
	get_address($t3,$t1)
	lw $t4,string($t2) #s[a]
	lw $t5,string($t3) #s[b]
	bne $t4,$t5,false
	addi $t0,$t0,1
	subi $t1,$t1,1
	blt $t0,$t1,check
	j true
	
false:
	li $a0,0
	li $v0,1
	syscall
	j end
true:
	li $a0,1
	li $v0,1
	syscall
	j end
end:
	li $v0,10
	syscall
```



# GCD最大公约数

```
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
```

# stringReverse

```
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
    move	$t0,	$a0	# 输入字符串	$t0 = string
        
ForLoop:
	bge	$s1,	$s2,	Exit
	la	$a0,	string
	move	$a1,	$s1
	move	$a2,	$s2	# 传参
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

```

