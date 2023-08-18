使用logisim搭建单周期CPU与添加指令
搭建
总设计
借用高老板的图，我们只需要分别做出PC、NPC、IM、RF、EXT、ALU、DM、Controller模块即可，再按图连线，最后进行控制信号的处理，一个CPU就差不多搭完了。目前支持的指令集为{addu、subu、ori、lw、sw、beq、jal、jr、nop、lui、sb、lb、sh、lh}

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114114561.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1NTUxOTMw,size_16,color_FFFFFF,t_70#pic_center)

下面分模块逐个分析

PC
本质上就是一个32位的寄存器，这里采用的是异步复位，所以直接把reset信号连在clear口。

![img](https://img-blog.csdnimg.cn/2020112711421427.png#pic_center)

NPC
由于我的CPU支持beq、jal、jr，所以NPCOp有2位，如下表所示

| NPCOp | 功能                 |
| ----- | -------------------- |
| 00    | 计算顺序地址（PC+4） |
| 01    | 计算beq地址          |
| 10    | 计算jal地址          |
| 11    | 计算jr地址           |

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114252628.png#pic_center)


其中PC4是用来把PC+4输出至RF以完成jal指令

![img](https://img-blog.csdnimg.cn/20201127114313844.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1NTUxOTMw,size_16,color_FFFFFF,t_70#pic_center)

IM
这个就更简单了，直接一个ROM搞定，注意把PC的2~6位引出作为IM的地址。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114405272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1NTUxOTMw,size_16,color_FFFFFF,t_70#pic_center)

RF
这个比较耗时间，听说用vscode打开.circ文件就可以写代码去搭建这玩意，打开后发现3k行代码，直接反手关掉vscode继续手动连线。连完应该差不多这个样子。



外部看起来是这样子的，RD1输出A1对应寄存器的值，RD2输出A2对应寄存器的值，当写使能信号WE3有效时，将在时钟上升沿把WD3写入A3对应的寄存器。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114438256.png#pic_center)

EXT
将imm16扩展后输出。由于我的lui是使用的EXT加载到高位（好像说其实应该用ALU实现？），因此我的EXT有3个功能分别是无符号扩展、符号扩展、左移16位。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114451728.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1NTUxOTMw,size_16,color_FFFFFF,t_70#pic_center)

ALU
根据ALUOp进行不同的运算即可，这里加法和减法用的一个加法器（A-B=A+~B+1）,但是好像这样不太好扩展（？）

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114510507.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1NTUxOTMw,size_16,color_FFFFFF,t_70#pic_center)

DM
本来是一个非常简单的RAM，但是由于做了lb、sb、lh、sh，就得对DM前后加上组合逻辑以保证不改变其他位的数据。这里用两个控制信号，SSel控制store时的位宽，LSel控制Load时的位宽。（其实用一个控制信号就可以，当时做的时候傻了一下用了两个）

使用一个译码器，选择被替换的那一段，如sb时，A的01位为01，那么就会将DM中的对应地址的32位数据中的815位替换成WD的低8位，再存入DM，这样就保证了仅读入一个字节而对其他位不改变，即实现了sb。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114528367.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1NTUxOTMw,size_16,color_FFFFFF,t_70#pic_center)

Controller
使用了最简朴的方法，搭建时可以先用小手把opcode和funct点成要添加的指令，然后再连接出该指令。如果opcode是000000，那么再与funct得出的信号并起来，即得到该指令，如图中的addu和subu。

![在这里插入图片描述](https://img-blog.csdnimg.cn/202011271145575.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1NTUxOTMw,size_16,color_FFFFFF,t_70#pic_center)

得到指令后，再根据列的真值表，把在真值表中是1的连上

如对于NPCOp[1:0]，只有beq时为01，只有jal时为10，只有jr时为11，也就是NPCOp[0]仅在beq和jr时取1，NPCOp[1]仅在jal和jr时取1，所以把他们连接起来即可，如下图。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114615212.png#pic_center)

对于每一个控制信号都如此连接，即可完成CPU的搭建。

添加指令
注意事项
在改变与门或者或门的输入数据个数时，建议从奇数个到奇数个，否则可能出现这样的情况：如我有一个5input的或门，如下


现在要添加第6个input，如果直接改成6input，发现中间的空掉了

而如果遵循奇数个改奇数个的原则就不会出错，如将number of input改成7就如下图所示，中间的连上了。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114709644.png#pic_center)

eg：添加addiu
首先分析数据通路
判断是否需要增加新的通路以实现该指令，可以看出其需要的功能我的CPU都有了，因此直接修改控制信号即可

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114722838.png#pic_center)

确定控制信号
对于NPCOp，这不是一个跳转指令，因此NPCOp取00

对于RFWr，要回写到R[rt]，因此RFWr为1

对于EXTOp，要进行符号扩展，所以取01

对于ALUOp，加法，所以取00

对于DMWr，不用写入DM，所以取0

对于WRSel，由于写入的是R[rt]，所以取01

对于WDSel，由于写入的数据来自ALU的计算结果，所以取00

对于BSel，由于参与ALU计算的第二个数来自EXT，所以取1

对于SSel和LSel，由于不涉及半字或字节，都取00

添加指令信号
首先将opcode点成该指令

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114735674.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1NTUxOTMw,size_16,color_FFFFFF,t_70#pic_center)

然后再连接出addiu

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114747992.png#pic_center)

这时候由于opcode即addiu指令，应该只有addiu是亮的。

修改控制信号
得到addiu信号后，仅需要在addiu控制信号为1的对应位置连线即可，如addiu只有RFWr、EXTOp[0]、WRSel[0]、BSel为1，所以只需要将他们的或门添加一根线与addiu信号连接。连接完成后，应检查一遍此时的控制信号是否与之前分析的一样。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114924744.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1NTUxOTMw,size_16,color_FFFFFF,t_70#pic_center)

可以看到与之前分析的完全一样，至此addiu的添加完成。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127114930634.png#pic_center)