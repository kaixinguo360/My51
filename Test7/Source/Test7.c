#include<reg51.h>   
#define uint unsigned int   
#define uchar unsigned char

sbit lcden = P1 ^ 5;
sbit rs = P1 ^ 7;
sbit rw = P1 ^ 6;
sbit busy = P2 ^ 7;
char i, isDone, temp, inNum, num_1;
long numA, numB, c;
float a_c, b_c;
uchar flag, fuHao;

uchar code table[] = {
	0,1,2,3,
	4,5,6,7,
	8,9,0,0,
	0,0,0,0
};
uchar code table1[] = {
	0,1,2,3,
	4,5,6,7,
	8,9,0x2b - 0x30,0x2d - 0x30,
	0x2a - 0x30,0x2f - 0x30,0x01 - 0x30,0x3d - 0x30
};

void delay(uchar z)  //延时子程序
{
	uchar y;
	for (z; z > 0; z--)
		for (y = 0; y < 110; y++);
}

void check()
{
	do {
		P2 = 0xFF;
		rs = 0;
		rw = 1;
		lcden = 0;
		delay(1);
		lcden = 1;
	} while (busy == 1);
}

void write_com(uchar com)  //写命令
{
	P2 = com;
	rs = 0;
	rw = 0;
	lcden = 0;
	check();
	lcden = 1;
}

void write_date(uchar date)  //写数据
{
	P2 = date;
	rs = 1;
	rw = 0;
	lcden = 0;
	check();
	lcden = 1;
}

void init()  //初始化
{
	inNum = -1;
	lcden = 1;
	write_com(0x01);  //清零
	write_com(0x38);  //设置16X2显示等
	write_com(0x0c);  //设置开显示，不显示光标
	write_com(0x06);  //写入新数据后，地址寄存器内容加1, 自动往下写
	write_com(0x80);  //第一行第一列开始 显示
	write_com(0x01);
	num_1 = 0;
	i = 0;
	isDone = 0;
	numA = 0;
	numB = 0;
	c = 0;
	flag = 0;
	fuHao = 0;
}

void keyscan()  //扫描键盘
{
    //扫描第1行
	P3 = 0xfe;
	if (P3 != 0xfe)
	{
		delay(5); //去除抖动
		if (P3 != 0xfe)
		{
			temp = P3 & 0xf0;
			switch (temp)
			{
			case 0xe0:
			    inNum = 0;
				break;
			case 0xd0:
			    inNum = 1;
				break;
			case 0xb0:
			    inNum = 2;
				break;
			case 0x70:
			    inNum = 3;
				break;
			}
		}
		while (P3 != 0xfe); //直到松开再继续运行
		if (inNum == 0 || inNum == 1 || inNum == 2 || inNum == 3)
		{
			if (isDone != 0) //如果计算完毕，则清除之前的结果，重新接收输入
			{
				write_com(0x01);
				isDone = 0;
			}
			if (flag == 0)
			{
				numA = numA * 10 + table[inNum];
			}
			else
			{
				numB = numB * 10 + table[inNum];
			}
		}
		i = table1[inNum];
		write_date(0x30 + i);
	}
	
    //扫描第2行
	P3 = 0xfd;
	if (P3 != 0xfd)
	{
		delay(5); //去除抖动
		if (P3 != 0xfd)
		{
			temp = P3 & 0xf0;
			switch (temp)
			{
			case 0xe0:
			    inNum = 4;
				break;
			case 0xd0:
			    inNum = 5;
				break;
			case 0xb0:
			    inNum = 6;
				break;
			case 0x70:
			    inNum = 7;
				break;
			}
		}
		while (P3 != 0xfd); //直到松开再继续运行
		if (inNum == 4 || inNum == 5 || inNum == 6 || inNum == 7)
		{
			if (isDone != 0) //如果计算完毕，则清除之前的结果，重新接收输入
			{
				write_com(0x01);
				isDone = 0;
			}
			if (flag == 0)
			{
				numA = numA * 10 + table[inNum];
			}
			else
			{
				numB = numB * 10 + table[inNum];
			}
		}

		i = table1[inNum];
		write_date(0x30 + i);
	}
	
    //扫描第3行
	P3 = 0xfb;
	if (P3 != 0xfb)
	{
		delay(5); //去除抖动
		if (P3 != 0xfb)
		{
			temp = P3 & 0xf0;
			switch (temp)
			{
			case 0xe0:
			    inNum = 8;
				break;
			case 0xd0:
			   inNum = 9;
				break;
			case 0xb0:
			   inNum = 10;
				break;
			case 0x70:
			   inNum = 11;
				break;
			}
		}
		while (P3 != 0xfb); //直到松开再继续运行
		if (inNum == 8 || inNum == 9)
		{
			if (isDone != 0) //如果计算完毕，则清除之前的结果，重新接收输入
			{
				write_com(0x01);
				isDone = 0;
			}
			if (flag == 0)
			{
				numA = numA * 10 + table[inNum];
			}
			else
			{
				numB = numB * 10 + table[inNum];
			}
		}
		if (inNum == 11) // -
		{
			flag = 1;
			fuHao = 2;
		}
		if (inNum == 10) // +
		{
			flag = 1;
			fuHao = 1;
		}
		i = table1[inNum];
		write_date(0x30 + i);
	}
	
    //扫描第4行
	P3 = 0xf7;
	if (P3 != 0xf7)
	{
		delay(5); //去除抖动
		if (P3 != 0xf7)
		{
			temp = P3 & 0xf0;
			switch (temp)
			{
			case 0xe0:
			    inNum = 12;
				break;
			case 0xd0:
			    inNum = 13;
				break;
			case 0xb0:
			    inNum = 14;
				break;
			case 0x70:
			    inNum = 15;
				break;
			}
		}
		while (P3 != 0xf7); //直到松开再继续运行
		switch (inNum)
		{
		case 12: // *
		{
		    write_date(0x30 + table1[inNum]);
		    flag = 1;
		    fuHao = 3;
		}
		break;
		case 13: // /
		{
		    write_date(0x30 + table1[inNum]);
		    flag = 1;
		    fuHao = 4;
		}
		break;
		case 15://计算结果
		{
		    isDone = 1; //设置计算完毕标志
			
			//加法
		    if (fuHao == 1) {
			    write_com(0x80 + 0x4f); //最后一个数的地址
			    write_com(0x04); //写入新数据后地址寄存器内容减1，从后往前显示
			    c = numA + numB; //计算
			    while (c != 0)
		    	{
				    write_date(0x30 + c % 10); //从个位开始依次显示各位
				    c = c / 10;
		    	}
			    write_date(0x3d); //显示等号  
		    	numA = 0; numB = 0; flag = 0; fuHao = 0; //清空变量
	    	}
			
			//减法
		    else if (fuHao == 2) {
			    write_com(0x80 + 0x4f);
			    write_com(0x04);
		    	if (numA - numB > 0) //计算
			    	c = numA - numB;
		    	else
			    	c = numB - numA;
			    while (c != 0)
		    	{
		    		write_date(0x30 + c % 10); //从个位开始依次显示各位
			    	c = c / 10;
		    	}
			    if (numA - numB < 0)
			    	write_date(0x2d); //显示负号(45)
		    	write_date(0x3d); //显示等号
		    	numA = 0; numB = 0; flag = 0; fuHao = 0; //清空变量
	    	}
			
			//乘法
		    else if (fuHao == 3) {
		    	write_com(0x80 + 0x4f);
		    	write_com(0x04);
		    	c = numA * numB; //计算
		    	while (c != 0)
		    	{
		    		write_date(0x30 + c % 10); //从个位开始依次显示各位
		    		c = c / 10;
		    	}
		    	write_date(0x3d); //显示等号
		    	numA = 0; numB = 0; flag = 0; fuHao = 0; //清空变量
	    	}
			
			//除法
		    else if (fuHao == 4) {
		    	write_com(0x80 + 0x4f);
		    	write_com(0x04);
		    	i = 0;
		    	if (numB == 0) //如果除数是0，则显示Error
			    {
			    	write_date('r');
			    	write_date('o');
		    		write_date('r');
		    		write_date('r');
		    		write_date('E');
		    	}
		    	else {
			    	c = (long)(((float)numA / numB) * 1000);
			    	while (c != 0)
			    	{
				    	write_date(0x30 + c % 10); //从个位开始依次显示各位
				    	c = c / 10;
				    	i++;
			    		if (i == 3)
			    			write_date(0x2e); //在第三位显示小数点(46)
			    	}
			    	if (numA / numB <= 0)
			    		write_date(0x30); //0
		    	}
		    	write_date(0x3d); //显示等号
		    	numA = 0; numB = 0; flag = 0; fuHao = 0; //清空变量
		    }
	    	break;
		}
		case 14:	//清除显示
		{
		    write_com(0x01); numA = 0; numB = 0; flag = 0; fuHao = 0; }
	    	break;
		}
	}
}

main()  //主函数
{
	init();  //初始化
	while (1)
	{
		keyscan();  //循环调用键盘扫描子程序
	}
}
