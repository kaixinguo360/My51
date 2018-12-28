#include <reg51.h>

#define uchar unsigned char
#define uint  unsigned int

uchar bm[]={0x1b,0x13,0x0b,0x03,0x1a,0x12,0x0a,0x02,0x19,0x11,0x09,0x01,0x18,0x10,0x08,0x00};

unsigned char digit[5];
unsigned char key_number, j, k,mk;    //mkΪ������������ֵ
unsigned int tmr;
unsigned long wait_cnter;

sbit cs=P1^0; 
sbit clk=P1^1; 
sbit dat=P1^2; 
sbit key=P1^3; 


void long_delay(void) // ����ʱ
{
	unsigned char i;
	for (i=0;i<0x30;i++);
}

void short_delay(void) // ������ʱ
{
	unsigned char i;
	for (i=0;i<8;i++);
}

void send_byte( unsigned char out_byte) // ����һ���ֽ�
{
	unsigned char i;
	cs=0;     //оƬʹ��
	long_delay();
	for (i=0;i<8;i++)   //��8����������
	{
		if (out_byte&0x80)   //�ȴ���λ
		{ 
			dat=1;
		}
		else
		{
			dat=0;
		}
		clk=1;
		short_delay();
		clk=0;
		short_delay();
		out_byte=out_byte*2;   //��������
	}
	dat=0;
}

unsigned char receive_byte(void) // ����һ���ֽ�
{
	unsigned char i, in_byte;
	dat=1; 
	long_delay();
	for (i=0;i<8;i++)   //��8�ζ������ݸ�λ��ǰ
	{
		clk=1;
		short_delay();
		in_byte=in_byte*2; //��������
		if (dat)
		{
			in_byte=in_byte|0x01;
		}
		clk=0;
		short_delay();
	}
	dat=0;
	return (in_byte);
}

void write7279(unsigned char cmd, unsigned char dta) // д�뵽HD7279
{
	send_byte (cmd);
	send_byte (dta);
} 

unsigned char read7279(unsigned char command) // ��HD7279����
{
	send_byte(command);
	return(receive_byte());
}

void main(){
	uchar keyboard, i, num;
	send_byte(0xa4);    //ȫ����λָ��
	while(1){
		if(key == 0){
			send_byte(0x15);           //������ָ��
			keyboard = receive_byte();   //���ռ�������
			// P0=num;
			for(i=0; i<16; i++){
				if(keyboard == bm[i]){
					num = i;
					break;
				}
			}
			send_byte(0xa1);
			write7279(0xc8, num);
			while(key == 0);
		}
	}  
}
