#include<reg51.h>    //������Ƭ���Ĵ�����ͷ�ļ�
#include<intrins.h>  //����_nop_()���������ͷ�ļ�

//1602�˿ڶ���
sbit RS=P2^0;    //�Ĵ���ѡ��λ����RSλ����ΪP2.0����
sbit RW=P2^1;    //��дѡ��λ����RWλ����ΪP2.1����
sbit E=P2^2;     //ʹ���ź�λ����Eλ����ΪP2.2����
sbit BF=P1^7;    //æµ��־λ������BFλ����ΪP0.7����

//AD�˿ڶ���
sbit CLK=P2^3;
sbit start=P2^4;
sbit oe=P2^5;
sbit eoc=P2^7;
sbit out_pulse=P2^6;//5us����
sbit p30=P3^0;

#define uchar unsigned char
#define uint unsigned int

uchar n=0;
uchar flag=0;//1s��־λ


//��ʱ1ms
void delay1ms()
{
   		unsigned char i,j;	
	 	for(i=0;i<10;i++)
	  	for(j=0;j<33;j++)
	   ;		 
 }

//��ʱ���ɺ���
 void delay(unsigned int n)
 {
   	unsigned int i;
	for(i=0;i<n;i++)
	delay1ms();
 }

//�ж�Һ��ģ���æµ״̬ (1-æµ 0-��æ)
 bit BusyTest(void)
  {
    	bit result;
		RS=0;       //���ݹ涨��RSΪ�͵�ƽ��RWΪ�ߵ�ƽʱ�����Զ�״̬
    	RW=1;
    	E=1;        //E=1���������д
    	_nop_();   //�ղ���
    	_nop_();
    	_nop_(); 
   		_nop_();   //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��	
    	result=BF;  //��æµ��־��ƽ����result
		E=0;
    	return result;
  }

//��ģʽ����ָ�����ʾ��ַд��Һ��ģ��
void Write_com (unsigned char dictate)
{   
    while(BusyTest()==1); //���æ�͵ȴ�
	 RS=0;                  //���ݹ涨��RS��R/WͬʱΪ�͵�ƽʱ������д��ָ��
	 RW=0;   
	 E=0;                   //E�õ͵�ƽ(дָ��ʱ��
                             // ������E��0��1���������䣬����Ӧ����"0"
	 _nop_();
	 _nop_();             //�ղ��������������ڣ���Ӳ����Ӧʱ��
	 P1=dictate;            //����������P0�ڣ���д��ָ����ַ
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	 E=1;                   //E�øߵ�ƽ
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	  E=0;                  //��E�ɸߵ�ƽ����ɵ͵�ƽʱ��Һ��ģ�鿪ʼִ������
 }

//ָ���ַ���ʾ��ʵ�ʵ�ַ
 void WriteAddress(unsigned char x)
 {
     Write_com(x|0x80); //��ʾλ�õ�ȷ�������涨Ϊ"80H+��ַ��x"	
 }

//������(�ַ��ı�׼ASCII��)д��Һ��ģ�� y(Ϊ�ַ�����)
 void WriteData(unsigned char y)
 {
    while(BusyTest()==1);  
	  RS=1;           //RSΪ�ߵ�ƽ��RWΪ�͵�ƽʱ������д������
	  RW=0;
	  E=0;            //E�õ͵�ƽ(дָ��ʱ����
                       // ������E��0��1���������䣬����Ӧ����"0"
	  P1=y;           //����������P0�ڣ���������д��Һ��ģ��
	  _nop_();
	  _nop_();
 	  _nop_();
    	  _nop_();       //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	  E=1;          //E�øߵ�ƽ
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();        //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	  E=0;            //��E�ɸߵ�ƽ����ɵ͵�ƽʱ��Һ��ģ�鿪ʼִ������
 }

//��LCD����ʾģʽ���г�ʼ������
void LcdInt()
{
   	delay(15);             //��ʱ15ms���״�дָ��ʱӦ��LCDһ�νϳ��ķ�Ӧʱ��
   	Write_com(0x38);  //��ʾģʽ���ã�16��2��ʾ��5��7����8λ���ݽӿ�
	delay(5);   //��ʱ5ms��

	Write_com(0x0c);  //��ʾģʽ���ã���ʾ�����й�꣬�����˸
	delay(5);
	Write_com(0x06);  
	delay(5);											  
	Write_com(0x01);  //����Ļָ�����ǰ����ʾ�������
	delay(5);
 }

//��ʼ��
void sysinit()
{
	LcdInt(); //��ʼ����Ļ
	delay(5);
	TMOD = 0x21; //�趨��ʱ��T1������ʽ  T0 ������ʽ1
	TH1=0xfd; //����T1�жϲ���CLK�ź�
	TL1=0xfd;//50khz
	EA = 1; //�����ж�  
	ET1=1;
	TR1=1; //������ʱ��T1
	TH0=0x45;
	TL0=0x00;//50MS��ʱ
	ET0=1;
	TR0=1;
	start=0;    // START�� A��Dת�������źţ����룬�ߵ�ƽ��Ч��
	oe=0;   //������������źţ����룬�ߵ�ƽ��Ч����A��Dת������ʱ���˶�����һ���ߵ�ƽ�����ܴ������̬�ţ������������
}

//T0�жϷ������
void t0() interrupt 1
{
	ET0=0;
	TH0=0X45;//46
	TL0=0X00;//50MS��ʱ
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

//ADC���ݲɼ�, ���زɼ�����
unsigned char adc()
{
   unsigned  char  Temp;
   start=1;
//	delay(1);
	start=0;  //�����ź�
   while (!eoc);// A��Dת�������ź�
	P0=0xff;
//	delay(1);
   oe=1;
   Temp=P0;//��ȡ�ɼ�����
   oe=0;
//  delay(4);
   return(Temp);//���زɼ�����
}

//��ʾ�Ӻ���
void display(uint a)
{
	uint bai,shi,ge;
	bai=a/100;
	shi=(a-bai*100)/10;
	ge=a%10;
	WriteAddress(2);
	WriteData(0x30+bai);//��ʾ��λ
	WriteData(0x30+shi);//��ʾʮλ
	WriteData(0x30+ge);//��ʾ��λ
	
}

//Main����
void main()
{
	uint temp1,temp2;
	uchar f=0;
	sysinit();
	CLK=0;
	WriteAddress(0);//�ӵ�0�п�ʼ��ʾ
	WriteData('f');//��ʾ f
	WriteData(':');//��ʾ ��
	WriteAddress(5);//�ӵ�5�п�ʼ��ʾ
	WriteData('H');	//��ʾH
	WriteData('z');//��ʾz
	while(1)
	{
		temp2=temp1;
		temp1=adc();//ad�ɼ�
		if(((temp1>=128)&&(temp2<=128))||((temp1<=128)&&(temp2>=128)))
		{
			f++;//
			out_pulse=1;
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			out_pulse=0;//����5us�������ź�
		}
		if(flag==1)//����1s�����Ƶ��
		{
			flag=0;
			f=f/2;
			display(f);//��ʾƵ��
			f=0;
		}
	}
}
