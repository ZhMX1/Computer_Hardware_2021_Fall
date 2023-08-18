# Logisim带有初始化功能的寄存器

## 子电路外观

![image](https://img2020.cnblogs.com/blog/2296368/202110/2296368-20211021143350585-1176192196.png)

## 子电路逻辑

![image](https://img2020.cnblogs.com/blog/2296368/202110/2296368-20211021143435687-1141478657.png)

描述：

- **Q端 Counter + MUX** ： 实现初始化。初始时MUX选择init，待时钟第一个上升沿后，Counter（[Data Bit]:1bit，[Action on Overflow]:Stay at value）达到最大值，Carry出1将MUX选择调回寄存器
- **Pull Register** ： 实现信号缺省设置。高阻态时上拉至设定电位。
- **D端 MUX** ： 实现enable置0时，保持寄存器内容。

![image-20211112174311550](C:\Users\zhmx\AppData\Roaming\Typora\typora-user-images\image-20211112174311550.png)

![image-20211112174323869](C:\Users\zhmx\AppData\Roaming\Typora\typora-user-images\image-20211112174323869.png)

为了保证正确性，把RAM设为下降沿写回（就是在时钟信号那里加一个非门)，在上升沿读取数据，处理完存到寄存器里，在下降沿由寄存器写入RAM
