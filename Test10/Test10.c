#include<reg51.h>
#include<stdio.h>
#include <absacc.h> // ��ʹ�����ж���ĺ������ʾ��Ե�ַ

#define uchar unsigned char
#define uint  unsigned int

#define PA    XBYTE[0x7FFC] //PAָ���ⲿ���ݴ洢��7FFCh�ĵ�ַ
#define COM   XBYTE[0x7FFF] //��P0 P2�����������ַ

#define EW_ON   0xDE //�����̵���
#define EW_OFF   0xDF //�����̵���
#define SN_ON   0xF3 //�ϱ��̵���
#define SN_OFF   0xFB //�ϱ��̵���

uint temp;
uchar a;

void init_8255()
{
	COM=0x80;
}

void delay_500ms() //��ʱ500MS�ӳ���
{
	uint i,j;
	for(i=500;i>0;i--)
		for(j=120;j>0;j--);
}

void delay_1s() //��ʱ1S�ӳ���
{
	uint i,j;
	for(i=1000;i>0;i--)
		for(j=120;j>0;j--);
}

void show(uint a) //��ʾ�ӳ���
{
	SCON=0x00; //����ģʽ0
	SBUF=a;    //������a��ֵ�͸�����Ĵ���
	while(!TI) //�ȴ������Ƿ����
		TI=0;  //�жϱ�־λ����
}

void main()
{
	init_8255();
	TMOD=0x01; //�趨����ģʽ1
	EA=1;      //�����ж�
	ET0=1;     //����T0�ж�
	TF0=0;     //��ʱ��0�����־λ
	while(1)
	{
		a=0x26;
		for(temp=25;temp>19;temp--) //�̵Ƶ���ʱ 25-20
		{
			a=a-1;
			PA=EW_ON; //�����̵���
			show(a);
			delay_1s();
		}
		a=0x1a;
		for(temp=19;temp>9;temp--) //�̵Ƶ���ʱ 19-10
		{
			a=a-1;
			PA=EW_ON;
			show(a);
			delay_1s();
		}
		a=0x0a;
		for(temp=9;temp>0;temp--) //�̵Ƶ���ʱ 9-0
		{
			a=a-1;
			PA=EW_ON;
			show(a);
			delay_1s();
		}
		for(temp=5;temp>0;temp--) //�̵���˸ 5s
		{
			a=0;     //����ʱֵΪ0ʱ��˸
			show(a);
			PA=EW_OFF; //�����̵���
			delay_500ms(); //��ʱ0.5s
			PA=EW_ON; //�����̵���
			delay_500ms(); //��ʱ0.5s
		}
		a=0x26;
		for(temp=25;temp>19;temp--) //�̵Ƶ���ʱ 25-20
		{
			a=a-1;
			PA=SN_ON; //�ϱ��̵���
			show(a);
			delay_1s();
		}
		a=0x1a;
		for(temp=19;temp>9;temp--) //�̵Ƶ���ʱ 19-10
		{
			a=a-1;
			PA=SN_ON;
			show(a);
			delay_1s();
		}
		a=0x0a;
		for(temp=9;temp>0;temp--) //�̵Ƶ���ʱ 9-0
		{
			a=a-1;
			PA=SN_ON;
			show(a);
			delay_1s();
		}
		for(temp=5;temp>0;temp--) //�̵���˸ 5s
		{
			a=0; //����ʱֵΪ0ʱ��˸
			show(a);
			PA=SN_OFF; //�ϱ��̵���
			delay_500ms(); //��ʱ0.5s
			PA=SN_ON; //�ϱ��̵���
			delay_500ms(); //��ʱ0.5s
		}          
	}
}
