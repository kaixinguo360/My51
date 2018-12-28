ORG 0000H
	AJMP MAIN

ORG 0030H
MAIN:
	MOV SP, #60H      ;设置堆栈指针
	MOV R0, #0        ;个位 = 0
	MOV R1, #1        ;十位 = 1
	CLR F0
	SETB P3.0         ;P3.0全部置1, LED灯熄灭

LOOP:
	JB P1.1, LOOP2    ;如果P1.1=1(RESET), 跳转到LOOP2，否则继续执行
LOOP1:                ;如果 P1.1不为1(RESET按下时)，则执行，装载初值，刷新显示，关掉LED1
	CLR F0
	MOV 30H, R0
	MOV 31H, R1       ;装载初值
	SETB P3.0         ;关闭LED1
	LCALL DISPLAY     ;显示
LOOP2:
	JB P1.0, LOOP     ;如果P1.0=1(START)，跳回LOOP，否则继续执行
LOOP3:                ;如果 P1.0不为1(START按下时)，则执行，开始倒计时
	LCALL DISPLAY     ;调用显示子程序
	LCALL DL1S        ;延时1s
	LCALL ADJUST      ;调用调整剩余时间子程序
	JB F0, LOOP4      ;如果F0=1, 跳转到LOOP4
	LJMP LOOP3        ;如果F0=0, 继续LOOP3

LOOP4:                ;LED闪烁程序
	CLR P3.0          ;点亮LED
	LCALL DL100MS     ;延时100ms
	SETB P3.0         ;熄灭LED
	LCALL DL100MS     ;延时100ms
	JB P1.1, LOOP4    ;如果P1.1=1, 继续LOOP4, 继续闪烁
	LJMP LOOP1        ;如果P1.1=0, 跳转到LOOP1, 结束闪烁

DISPLAY:              ;显示子程序   
	MOV A, 30H
	MOV DPTR, #TABLE
	MOVC A, @A+DPTR   ;读取个位数字对应编码
	MOV DPTR, #U1
	MOVX @DPTR, A     ;U1 = 个位数字对应编码
	MOV A, 31H
	MOV DPTR, #TABLE
	MOVC A, @A+DPTR   ;读取十位数字对应编码
	MOV DPTR, #U10
	MOVX @DPTR, A     ;U1 = 十位数字对应编码
	RET

ADJUST:
	DEC 30H           ;个位数字 - 1
	MOV A, 30H
	CJNE A, #-1, GORET;如果(个位数字 != -1), 则返回
	MOV 30H, #9       ;如果(个位数字 == -1), 设个位数字为9, 并继续运行
	DEC 31H           ;十位数字 - 1
	MOV A, 31H
	CJNE A, #-1, GORET;如果(十位数字 != -1), 则返回
	SETB F0           ;如果(十位数字 == -1), 设把F0设为1
	RET
GORET:
	RET

DL1S:                 ;三重循环延时1s子程序
    MOV R7, #10
DL2:
    MOV R6, #200
DL1:
    MOV R5, #250
    DJNZ R5, $
	DJNZ R6, DL1
	DJNZ R7, DL2
	RET

DL100MS:              ;两重循环延时100ms子程序
    MOV R7, #200
DL:
    MOV R6, #250
    DJNZ R6, $
    DJNZ R7, DL
    RET

TABLE:
    DB 0C0H, 0f9H, 0A4H, 0B0H, 99H, 92H, 82H, 0F8H, 80H, 90H

U1 EQU 0FE00H   ;个位数码管的地址
U10 EQU 0FD00H  ;十位数码管的地址

END
