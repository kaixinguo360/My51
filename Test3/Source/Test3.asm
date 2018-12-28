ORG 0000H
    AJMP MAIN

ORG 0030H
MAIN:
    MOV SP, #60H     ;设置堆栈指针
    MOV P2, #0FEH    ;将P2除P2.0外全置为高电平
LOOP1:
    JB P1.0, LOOP1   ;P1.0位为0，则继续执行, 否则在此语句循环
    MOV A, P2        ;将P2的值赋给A
    RL A             ;将A循环左移
    MOV P2,A         ;将A的值赋给P2
    ACALL DL100ms    ;调用延时100ms子程序
    SJMP LOOP1       ;继续循环

DL100ms:             ;双重循环延时100ms子程序
    MOV R7, #200     ;t = (2 * 250) * 200 = 100000us = 100ms
DL: MOV R6, #250
    DJNZ R6, $
    DJNZ R7, DL
    RET
END
