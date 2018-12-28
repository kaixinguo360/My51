#include<reg52.h>

#define uint unsigned int
#define uchar unsigned char

sbit LED1 = P0^0; //用P3^0来控制led
sbit BUZZER = P0^1; //用p3.1口控制蜂鸣器
sbit SKEY1 = P3^2; //按键
uint count_int; //计数指针

void delay(uint c) //延时1ms
{
	uint x, y;
	for(x=c; x>0; x--)
		for(y=110; y>0; y--); 
}

void control(uint count_int) 
{
	uint i;
	for(i=0; i<=count_int; i++) //函数循环i次
	{
		LED1 = 0;
		BUZZER = 0; //灯亮，蜂鸣器响 
		delay(500); //延时0.5s
		LED1 = 1;
		BUZZER = 1;  
		delay(500); 
	}
}

void s_timer0() interrupt 0 using 0 //中断0
{
	EA = 0; //屏蔽其他中断请求 
	control(count_int); //调用子程序 
	count_int++; 
	delay(50);
	if(count_int >= 10) //若count为十则结束
		count_int = 0;
	EA = 1; //开放中断
}

void main()
{
	EA = 1; //开放中断
	EX0 = 1; //允许外部中断0中断 
	IT0 = 0; //外部中断0为电平触发方式 02
	while(1); //循环执行，等待循环
}
