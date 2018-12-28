ORG 0000H
    AJMP MAIN

ORG 0030H
MAIN:
    MOV SP, #60H   ;设置堆栈指针
    MOV P0, #0FFH  ;设P0为高电平(低电平有效)
    MOV P1, #0FFH  ;设P1为高电平(低电平有效)
    MOV R7, #10    ;速度变量
    MOV R4, #0EFH  ;速度指示灯变量

    LCALL SCAN     ;调用SCAN子程序
    JMP MAIN

BUTTON0:           ;正转
    MOV 30H, #0H   ;(30H) = 0
    MOV P1, #0FEH
    JMP Z_ZHANG    ;跳转到正转语句

BUTTON1:           ;反转
    MOV 30H,#1H    ;(30H) = 1
    MOV P1,#0FEH	
    JMP Z_FAN      ;跳转到反转语句

BUTTON2:           ;加速
    DEC R7         ;R7 = R7-1
    DEC R7         ;R7 = R7-1
    MOV A,R4       ;...
    RR A           ;...
    MOV R4,A       ;移动速度指示灯变量
    MOV A,30H      ;...
    JZ Z_ZHANG     ;若(30H)=0, 则跳转到正转语句
    JMP Z_FAN      ;若(30H)=1, 则跳转到反转语句

BUTTON3:           ;减速
    INC R7         ;R7 = R7+1
    INC R7         ;R7 = R7+1
    MOV A,R4       ;...
    RL A           ;...
    MOV R4,A       ;移动速度指示灯变量
    MOV A,30H
    JZ Z_ZHANG     ;若(30H)=0, 则跳转到正转语句
    JMP Z_FAN      ;若(30H)=1, 则跳转到反转语句

Z_ZHANG:	
    MOV P0,R4
    LCALL DLS      ;调用延时子程序
    MOV A,P1
    RL A           ;左移一位
    MOV P1,A       ;...
    LCALL SCAN     ;调用SCAN子程序
    JMP Z_ZHANG    ;跳转到正转语句, 继续正转

Z_FAN:	
    MOV P0,R4
    LCALL DLS      ;调用延时子程序
    MOV A,P1
    RR A           ;右移一位
    MOV P1,A
    LCALL SCAN     ;调用SCAN子程序
    JMP Z_FAN      ;跳转到反转语句, 继续反转

SCAN:                 ;扫描法读取输入子程序，将全部行线置低电平，检测列线的状态，只要有一列电平为低，则表示键盘中有键按下
    MOV P3, #030H
    JNB P3.4, NEXT1
    JNB P3.5, NEXT2
    JMP GORET
NEXT1:	          
    MOV P3, #3H
    JNB P3.0, BUTTON0
    JNB P3.1, BUTTON2
    JMP GORET
NEXT2:            
    MOV P3, #3H
    JNB P3.0, BUTTON1
    JNB P3.1, BUTTON3
GORET:
    RET

DLS:               ;延时子程序
    MOV A,R7       ;t = (2 * 250) * 200 * R7 = 1000,000 * R7 = 1s * R7
DL2:
    MOV R6,#200
DL1:
    MOV R5,#250
DL0:
    DJNZ R5,DL0
    DJNZ R6,DL1
    DJNZ R7,DL2
    MOV R7,A
    RET

END
