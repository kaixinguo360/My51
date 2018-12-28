#include<reg51.h>    //包含单片机寄存器的头文件
#include<intrins.h>  //包含_nop_()函数定义的头文件

//1602端口定义
sbit RS=P2^0;    //寄存器选择位，将RS位定义为P2.0引脚
sbit RW=P2^1;    //读写选择位，将RW位定义为P2.1引脚
sbit E=P2^2;     //使能信号位，将E位定义为P2.2引脚
sbit BF=P1^7;    //忙碌标志位，，将BF位定义为P0.7引脚

//AD端口定义
sbit CLK=P2^3;
sbit start=P2^4;
sbit oe=P2^5;
sbit eoc=P2^7;
sbit out_pulse=P2^6;//5us脉冲
sbit p30=P3^0;

#define uchar unsigned char
#define uint unsigned int

uchar n=0;
uchar flag=0;//1s标志位


//延时1ms
void delay1ms()
{
   		unsigned char i,j;	
	 	for(i=0;i<10;i++)
	  	for(j=0;j<33;j++)
	   ;		 
 }

//延时若干毫秒
 void delay(unsigned int n)
 {
   	unsigned int i;
	for(i=0;i<n;i++)
	delay1ms();
 }

//判断液晶模块的忙碌状态 (1-忙碌 0-不忙)
 bit BusyTest(void)
  {
    	bit result;
		RS=0;       //根据规定，RS为低电平，RW为高电平时，可以读状态
    	RW=1;
    	E=1;        //E=1，才允许读写
    	_nop_();   //空操作
    	_nop_();
    	_nop_(); 
   		_nop_();   //空操作四个机器周期，给硬件反应时间	
    	result=BF;  //将忙碌标志电平赋给result
		E=0;
    	return result;
  }

//将模式设置指令或显示地址写入液晶模块
void Write_com (unsigned char dictate)
{   
    while(BusyTest()==1); //如果忙就等待
	 RS=0;                  //根据规定，RS和R/W同时为低电平时，可以写入指令
	 RW=0;   
	 E=0;                   //E置低电平(写指令时，
                             // 就是让E从0到1发生正跳变，所以应先置"0"
	 _nop_();
	 _nop_();             //空操作两个机器周期，给硬件反应时间
	 P1=dictate;            //将数据送入P0口，即写入指令或地址
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //空操作四个机器周期，给硬件反应时间
	 E=1;                   //E置高电平
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //空操作四个机器周期，给硬件反应时间
	  E=0;                  //当E由高电平跳变成低电平时，液晶模块开始执行命令
 }

//指定字符显示的实际地址
 void WriteAddress(unsigned char x)
 {
     Write_com(x|0x80); //显示位置的确定方法规定为"80H+地址码x"	
 }

//将数据(字符的标准ASCII码)写入液晶模块 y(为字符常量)
 void WriteData(unsigned char y)
 {
    while(BusyTest()==1);  
	  RS=1;           //RS为高电平，RW为低电平时，可以写入数据
	  RW=0;
	  E=0;            //E置低电平(写指令时，，
                       // 就是让E从0到1发生正跳变，所以应先置"0"
	  P1=y;           //将数据送入P0口，即将数据写入液晶模块
	  _nop_();
	  _nop_();
 	  _nop_();
    	  _nop_();       //空操作四个机器周期，给硬件反应时间
	  E=1;          //E置高电平
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();        //空操作四个机器周期，给硬件反应时间
	  E=0;            //当E由高电平跳变成低电平时，液晶模块开始执行命令
 }

//对LCD的显示模式进行初始化设置
void LcdInt()
{
   	delay(15);             //延时15ms，首次写指令时应给LCD一段较长的反应时间
   	Write_com(0x38);  //显示模式设置：16×2显示，5×7点阵，8位数据接口
	delay(5);   //延时5ms　

	Write_com(0x0c);  //显示模式设置：显示开，有光标，光标闪烁
	delay(5);
	Write_com(0x06);  
	delay(5);											  
	Write_com(0x01);  //清屏幕指令，将以前的显示内容清除
	delay(5);
 }

//初始化
void sysinit()
{
	LcdInt(); //初始化屏幕
	delay(5);
	TMOD = 0x21; //设定定时器T1工作方式  T0 工作方式1
	TH1=0xfd; //利用T1中断产生CLK信号
	TL1=0xfd;//50khz
	EA = 1; //开总中断  
	ET1=1;
	TR1=1; //启动定时器T1
	TH0=0x45;
	TL0=0x00;//50MS定时
	ET0=1;
	TR0=1;
	start=0;    // START： A／D转换启动信号，输入，高电平有效。
	oe=0;   //数据输出允许信号，输入，高电平有效。当A／D转换结束时，此端输入一个高电平，才能打开输出三态门，输出数字量。
}

//T0中断服务程序
void t0() interrupt 1
{
	ET0=0;
	TH0=0X45;//46
	TL0=0X00;//50MS定时
	n++;
	if(n==20) {flag=1;n=0;}
	ET0=1;
}

void t1() interrupt 3
{
	ET1=0;
	CLK=~CLK;
	ET1=1;
}

//ADC数据采集, 返回采集数据
unsigned char adc()
{
   unsigned  char  Temp;
   start=1;
//	delay(1);
	start=0;  //启动信号
   while (!eoc);// A／D转换结束信号
	P0=0xff;
//	delay(1);
   oe=1;
   Temp=P0;//读取采集数据
   oe=0;
//  delay(4);
   return(Temp);//返回采集数据
}

//显示子函数
void display(uint a)
{
	uint bai,shi,ge;
	bai=a/100;
	shi=(a-bai*100)/10;
	ge=a%10;
	WriteAddress(2);
	WriteData(0x30+bai);//显示百位
	WriteData(0x30+shi);//显示十位
	WriteData(0x30+ge);//显示个位
	
}

//Main函数
void main()
{
	uint temp1,temp2;
	uchar f=0;
	sysinit();
	CLK=0;
	WriteAddress(0);//从第0行开始显示
	WriteData('f');//显示 f
	WriteData(':');//显示 ：
	WriteAddress(5);//从第5行开始显示
	WriteData('H');	//显示H
	WriteData('z');//显示z
	while(1)
	{
		temp2=temp1;
		temp1=adc();//ad采集
		if(((temp1>=128)&&(temp2<=128))||((temp1<=128)&&(temp2>=128)))
		{
			f++;//
			out_pulse=1;
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			out_pulse=0;//产生5us的脉冲信号
		}
		if(flag==1)//到达1s后计算频率
		{
			flag=0;
			f=f/2;
			display(f);//显示频率
			f=0;
		}
	}
}
