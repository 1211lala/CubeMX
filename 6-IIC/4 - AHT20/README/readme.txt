
							气体代码 		自己定义ID

	1	一氧化碳（CO）		0x01			0x01		0-1000ppm		ppm
	
*	2	氧气（O2）			0x04			0x02		0-30.0%VOL		%VOL

*	3	二氧化碳（CO2）		0x12			0x03		0-5.0%VOL		%VOL
	4	硫化氢（H2S）		0x02			0x04		0-100ppm		ppm
	
*	5	甲烷(CH4)			0x06			0x05		0-5.0%VOL			%VOL

	6	二氧化硫(SO2)		0x05			0x06		0-20ppm			ppm
	7	氨气 (NH3)			0x0A			0x07		0-100ppm		ppm
	8	甲苯 (C7H8)			0x16			0x08		手册上未定义，在问厂家	
	9	乙醛 C2H4O			0x3C 			0x09		手册上未定义，在问厂家	
	10	可燃气体 (EX)		0x03			0x0a		0-100%LEL		%LEL




	读取传感器ID(自己设置)  
	FF 03 20 11 00 01 ca 11   
	读取气体名称代码   
	FF 03 20 27 00 01 2a 1f  
	读取单位代码 
	FF 03 20 30 00 01 9a 1b
	读取量程  
	FF 03 20 2b 00 01 ea 1c  
	读取浓度值(数据)  
	FF 03 60 01 00 01 DE 14   
	读取小数点值 
	FF 03 20 31 00 01 cb db   
	读取报警状态  (15位为预热完成位 为1 完成 0未完成)
	FF 03 60 00 00 01 8f d4   

	printf("气体名称					对应ID\r\n");
	printf("一氧化碳					0x01\r\n");
	printf("氧气					0x02\r\n");
	printf("二氧化碳					0x03\r\n");
	printf("硫化氢					0x04\r\n");
	printf("甲烷					0x05\r\n");
	printf("二氧化硫					0x06\r\n");
	printf("氨气					0x07\r\n");
	printf("甲苯					0x08\r\n");
	printf("乙醛					0x09\r\n");
	printf("可燃气体				0x0a\r\n\r\n");

	
//		c1 = aht20_buf[0]*100*10/1024/1024;  //计算得到湿度值c1（放大了10倍）
//		t1 = aht20_buf[1]*200*10/1024/1024-500;//计算得到温度值t1（放大了10倍）





