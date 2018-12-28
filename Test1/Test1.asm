ORG 0000H
    AJMP MAIN        ;跳转MAIN函数
ORG 0030H

MAIN:                ;初始化变量
    MOV R0,#20H      ;R0 - 剩余循环次数(20H -1)
    MOV R1,#30H      ;R1 - 数据保存位置(30H +1)
    MOV DPTR,#TABLE  ;DPTR - 表格地址
    MOV R2,#00H      ;R2 - 表格内偏移(0 +1)

GET:                 ;读取TABLE
    MOV A,R2         ;...
    MOVC A,@A+DPTR   ;读取TABLE中R2位置数据
    MOV @R1,A        ;保存此数据到@R1
    INC R1           ;R1 +1
    INC R2           ;R2 +1
    DJNZ R0,GET      ;如果剩余循环次数不为0, 则重复GET

    MOV R3,#1FH      ;R3 - 内层循环次数
    MOV R4,#1FH      ;R4 - 外层循环次数
    MOV R1,#30H      ;R1 - 数据指针


L1:
    CLR C            ;清除进位标志
    MOV A,@R1        ;读取R1指针所指示的数据到A
    MOV R5,A         ;暂存A到R5
    INC R1           ;R1 +1
    SUBB A,@R1       ;计算A-@R1, 借位结果保存到C
    MOV A,R5         ;恢复A为R5
    JC L2            ;如果C=1(A<@R1), 则继续运行

                     ;如果C=0(A>=@R1), 则交换R1与R1-1
    XCH A,@R1        ;交换: A<->R1
    DEC R1           ;R1 -1
    MOV @R1,A        ;MOV: 当前处理的数据 -> R1
    INC R1           ;R1 +1

L2:
    DJNZ R3,L1       ;如果内层剩余循环次数不为零, 则继续循环

                     ;如果内层剩余循环次数为零, 则调整参数重新开始内层循环
    MOV B,R4         ;...
    MOV R3,B         ;...
    DEC R3           ;内层循环次数 = 外层循环次数 - 1
    MOV R1,#30H      ;恢复数据指针到30H
    DJNZ R4,L1       ;如果外层剩余循环次数不为零, 则继续循环

ORG 3000H
TABLE:
    DB 1,3,9,2,17,4,11,6
    DB 5,20,100,64,21,14,79,35
    DB 92,7,91,23,65,16,13,18
    DB 18,73,65,101,27,19,62,69

SJMP $
END