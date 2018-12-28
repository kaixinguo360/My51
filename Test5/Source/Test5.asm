ORG 0000H
	AJMP MAIN

ORG 0030H
MAIN:
	MOV SP, #60H      ;���ö�ջָ��
	MOV R0, #0        ;��λ = 0
	MOV R1, #1        ;ʮλ = 1
	CLR F0
	SETB P3.0         ;P3.0ȫ����1, LED��Ϩ��

LOOP:
	JB P1.1, LOOP2    ;���P1.1=1(RESET), ��ת��LOOP2���������ִ��
LOOP1:                ;��� P1.1��Ϊ1(RESET����ʱ)����ִ�У�װ�س�ֵ��ˢ����ʾ���ص�LED1
	CLR F0
	MOV 30H, R0
	MOV 31H, R1       ;װ�س�ֵ
	SETB P3.0         ;�ر�LED1
	LCALL DISPLAY     ;��ʾ
LOOP2:
	JB P1.0, LOOP     ;���P1.0=1(START)������LOOP���������ִ��
LOOP3:                ;��� P1.0��Ϊ1(START����ʱ)����ִ�У���ʼ����ʱ
	LCALL DISPLAY     ;������ʾ�ӳ���
	LCALL DL1S        ;��ʱ1s
	LCALL ADJUST      ;���õ���ʣ��ʱ���ӳ���
	JB F0, LOOP4      ;���F0=1, ��ת��LOOP4
	LJMP LOOP3        ;���F0=0, ����LOOP3

LOOP4:                ;LED��˸����
	CLR P3.0          ;����LED
	LCALL DL100MS     ;��ʱ100ms
	SETB P3.0         ;Ϩ��LED
	LCALL DL100MS     ;��ʱ100ms
	JB P1.1, LOOP4    ;���P1.1=1, ����LOOP4, ������˸
	LJMP LOOP1        ;���P1.1=0, ��ת��LOOP1, ������˸

DISPLAY:              ;��ʾ�ӳ���   
	MOV A, 30H
	MOV DPTR, #TABLE
	MOVC A, @A+DPTR   ;��ȡ��λ���ֶ�Ӧ����
	MOV DPTR, #U1
	MOVX @DPTR, A     ;U1 = ��λ���ֶ�Ӧ����
	MOV A, 31H
	MOV DPTR, #TABLE
	MOVC A, @A+DPTR   ;��ȡʮλ���ֶ�Ӧ����
	MOV DPTR, #U10
	MOVX @DPTR, A     ;U1 = ʮλ���ֶ�Ӧ����
	RET

ADJUST:
	DEC 30H           ;��λ���� - 1
	MOV A, 30H
	CJNE A, #-1, GORET;���(��λ���� != -1), �򷵻�
	MOV 30H, #9       ;���(��λ���� == -1), ���λ����Ϊ9, ����������
	DEC 31H           ;ʮλ���� - 1
	MOV A, 31H
	CJNE A, #-1, GORET;���(ʮλ���� != -1), �򷵻�
	SETB F0           ;���(ʮλ���� == -1), ���F0��Ϊ1
	RET
GORET:
	RET

DL1S:                 ;����ѭ����ʱ1s�ӳ���
    MOV R7, #10
DL2:
    MOV R6, #200
DL1:
    MOV R5, #250
    DJNZ R5, $
	DJNZ R6, DL1
	DJNZ R7, DL2
	RET

DL100MS:              ;����ѭ����ʱ100ms�ӳ���
    MOV R7, #200
DL:
    MOV R6, #250
    DJNZ R6, $
    DJNZ R7, DL
    RET

TABLE:
    DB 0C0H, 0f9H, 0A4H, 0B0H, 99H, 92H, 82H, 0F8H, 80H, 90H

U1 EQU 0FE00H   ;��λ����ܵĵ�ַ
U10 EQU 0FD00H  ;ʮλ����ܵĵ�ַ

END
