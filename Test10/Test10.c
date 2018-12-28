#include<reg51.h>
#include<stdio.h>
#include <absacc.h> // 可使用其中定义的宏来访问绝对地址

#define uchar unsigned char
#define uint  unsigned int

#define PA    XBYTE[0x7FFC] //PA指代外部数据存储器7FFCh的地址
#define COM   XBYTE[0x7FFF] //由P0 P2口联合输出地址

#define EW_ON   0xDE //东西绿灯亮
#define EW_OFF   0xDF //东西绿灯灭
#define SN_ON   0xF3 //南北绿灯亮
#define SN_OFF   0xFB //南北绿灯灭

uint temp;
uchar a;

void init_8255()
{
	COM=0x80;
}

void delay_500ms() //延时500MS子程序
{
	uint i,j;
	for(i=500;i>0;i--)
		for(j=120;j>0;j--);
}

void delay_1s() //延时1S子程序
{
	uint i,j;
	for(i=1000;i>0;i--)
		for(j=120;j>0;j--);
}

void show(uint a) //显示子程序
{
	SCON=0x00; //工作模式0
	SBUF=a;    //将变量a的值送给缓存寄存器
	while(!TI) //等待发送是否完成
		TI=0;  //中断标志位清零
}

void main()
{
	init_8255();
	TMOD=0x01; //设定工作模式1
	EA=1;      //开放中断
	ET0=1;     //允许T0中断
	TF0=0;     //定时器0溢出标志位
	while(1)
	{
		a=0x26;
		for(temp=25;temp>19;temp--) //绿灯倒计时 25-20
		{
			a=a-1;
			PA=EW_ON; //东西绿灯亮
			show(a);
			delay_1s();
		}
		a=0x1a;
		for(temp=19;temp>9;temp--) //绿灯倒计时 19-10
		{
			a=a-1;
			PA=EW_ON;
			show(a);
			delay_1s();
		}
		a=0x0a;
		for(temp=9;temp>0;temp--) //绿灯倒计时 9-0
		{
			a=a-1;
			PA=EW_ON;
			show(a);
			delay_1s();
		}
		for(temp=5;temp>0;temp--) //绿灯闪烁 5s
		{
			a=0;     //当计时值为0时闪烁
			show(a);
			PA=EW_OFF; //东西绿灯灭
			delay_500ms(); //延时0.5s
			PA=EW_ON; //东西绿灯亮
			delay_500ms(); //延时0.5s
		}
		a=0x26;
		for(temp=25;temp>19;temp--) //绿灯倒计时 25-20
		{
			a=a-1;
			PA=SN_ON; //南北绿灯亮
			show(a);
			delay_1s();
		}
		a=0x1a;
		for(temp=19;temp>9;temp--) //绿灯倒计时 19-10
		{
			a=a-1;
			PA=SN_ON;
			show(a);
			delay_1s();
		}
		a=0x0a;
		for(temp=9;temp>0;temp--) //绿灯倒计时 9-0
		{
			a=a-1;
			PA=SN_ON;
			show(a);
			delay_1s();
		}
		for(temp=5;temp>0;temp--) //绿灯闪烁 5s
		{
			a=0; //当计时值为0时闪烁
			show(a);
			PA=SN_OFF; //南北绿灯灭
			delay_500ms(); //延时0.5s
			PA=SN_ON; //南北绿灯亮
			delay_500ms(); //延时0.5s
		}          
	}
}
