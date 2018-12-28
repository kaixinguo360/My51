#include <reg51.h>
#include <stdio.h>

#define uchar unsigned char
#define uint unsigned char

uchar data a[32] _at_ 0x30;	   
uint i, j;

void main() {  

    //赋初始值
	a[0] = 1;
	a[1] = 9;
	a[2] = 2;
	a[3] = 10;
	a[4] = 8;
	a[5] = 3;
	a[6] = 17;
	a[7] = 7;
	a[8] = 16;
	a[9] = 4;
	a[10] = 6;
	a[11] = 18;
	a[12] = 11;
	a[13] = 5;
	a[14] = 14;
	a[15] = 12;
	a[16] = 15;
	a[17] = 25;
	a[18] = 13;
	a[19] = 24;
	a[20] = 23;
	a[21] = 26;
	a[22] = 19;
	a[23] = 22;
	a[24] = 28;
	a[25] = 27;
	a[26] = 21;
	a[27] = 20;
	a[28] = 31;
	a[29] = 30;
	a[30] = 32;
	a[31] = 29;

	//冒泡法排序
	for(i=0; i<32; i++) {  //外层循环(32次)
	    for(j=0; j<32-1-i; j++) {  //内层循环(32-1-i次)
		    if(a[j] > a[j+1]) {  //如果前一项小于后一项, 则交换他们
			    uchar tmp = a[j+1];
				a[j+1] = a[j];
				a[j] = tmp;
			}
		}
	}

	while(1) {} //使程序停止
}
