#include<reg52.h>

#define uint unsigned int
#define uchar unsigned char

sbit LED1 = P0^0; //��P3^0������led
sbit BUZZER = P0^1; //��p3.1�ڿ��Ʒ�����
sbit SKEY1 = P3^2; //����
uint count_int; //����ָ��

void delay(uint c) //��ʱ1ms
{
	uint x, y;
	for(x=c; x>0; x--)
		for(y=110; y>0; y--); 
}

void control(uint count_int) 
{
	uint i;
	for(i=0; i<=count_int; i++) //����ѭ��i��
	{
		LED1 = 0;
		BUZZER = 0; //�������������� 
		delay(500); //��ʱ0.5s
		LED1 = 1;
		BUZZER = 1;  
		delay(500); 
	}
}

void s_timer0() interrupt 0 using 0 //�ж�0
{
	EA = 0; //���������ж����� 
	control(count_int); //�����ӳ��� 
	count_int++; 
	delay(50);
	if(count_int >= 10) //��countΪʮ�����
		count_int = 0;
	EA = 1; //�����ж�
}

void main()
{
	EA = 1; //�����ж�
	EX0 = 1; //�����ⲿ�ж�0�ж� 
	IT0 = 0; //�ⲿ�ж�0Ϊ��ƽ������ʽ 02
	while(1); //ѭ��ִ�У��ȴ�ѭ��
}
