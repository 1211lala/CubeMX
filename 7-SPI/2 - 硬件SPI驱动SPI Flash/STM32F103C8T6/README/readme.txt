/*****************************************************************

W25Q128的SPI模式为模式0或模式3。这里采用模式0
数据格式为8位，第一个bit为MSB

一个W25Q128有256的块
一个块有16个扇区 	(扇区 == 最小的的擦除单位)
一个扇区有16个页 	(页 == 最小的的写入单位)
一个页有256个字节 

地址从 0x000000 - 0xffffff

读状态寄存器SR1 
	拉低CS --> 发送0x05指令 --> 接收SR1的值 --> 拉高片选
	
页写时序
	拉低CS --> 发送0x02指令 --> 发送24位地址 --> 发送n个数据(n的范围为1-256) --> 拉高片选 
	
扇区擦除指令
	拉低CS --> 发送0x20指令 --> 发送24未地址 --> 拉高片选


在参考手册13页
状态寄存器SR1的值 bit0(BUSY) 0：空闲状态	1：忙碌状态
状态寄存器SR1的值 bit1(WEL) 0：写禁止	1：写使能

D0 --- MISO
D1 --- MOSI


注意 在向W25Q128写入大量数据(0.5kb - nkb之间要考虑有没有可能栈溢出)
stm32CubeMX默认的栈大小为 512b
写数据时因为定义了一个4k的全局数组，所以不用考虑
在读数据时要特别注意，比如要读一个1k字节的数据，就要把栈大小设置成2K左右(我是这样设置的，可以正常使用，小了就不能正常读数据，同时程序也卡死)


STM32F1的SPI1的速度手册上规定最快18Mhz,实测二分屏36Mhz也可以，但是稳定还是18Mhz
*****************************************************************/
