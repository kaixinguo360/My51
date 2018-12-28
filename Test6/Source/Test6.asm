ORG 0000H
    AJMP MAIN

ORG 0030H
MAIN:
    MOV SP, #60H   ;���ö�ջָ��
    MOV P0, #0FFH  ;��P0Ϊ�ߵ�ƽ(�͵�ƽ��Ч)
    MOV P1, #0FFH  ;��P1Ϊ�ߵ�ƽ(�͵�ƽ��Ч)
    MOV R7, #10    ;�ٶȱ���
    MOV R4, #0EFH  ;�ٶ�ָʾ�Ʊ���

    LCALL SCAN     ;����SCAN�ӳ���
    JMP MAIN

BUTTON0:           ;��ת
    MOV 30H, #0H   ;(30H) = 0
    MOV P1, #0FEH
    JMP Z_ZHANG    ;��ת����ת���

BUTTON1:           ;��ת
    MOV 30H,#1H    ;(30H) = 1
    MOV P1,#0FEH	
    JMP Z_FAN      ;��ת����ת���

BUTTON2:           ;����
    DEC R7         ;R7 = R7-1
    DEC R7         ;R7 = R7-1
    MOV A,R4       ;...
    RR A           ;...
    MOV R4,A       ;�ƶ��ٶ�ָʾ�Ʊ���
    MOV A,30H      ;...
    JZ Z_ZHANG     ;��(30H)=0, ����ת����ת���
    JMP Z_FAN      ;��(30H)=1, ����ת����ת���

BUTTON3:           ;����
    INC R7         ;R7 = R7+1
    INC R7         ;R7 = R7+1
    MOV A,R4       ;...
    RL A           ;...
    MOV R4,A       ;�ƶ��ٶ�ָʾ�Ʊ���
    MOV A,30H
    JZ Z_ZHANG     ;��(30H)=0, ����ת����ת���
    JMP Z_FAN      ;��(30H)=1, ����ת����ת���

Z_ZHANG:	
    MOV P0,R4
    LCALL DLS      ;������ʱ�ӳ���
    MOV A,P1
    RL A           ;����һλ
    MOV P1,A       ;...
    LCALL SCAN     ;����SCAN�ӳ���
    JMP Z_ZHANG    ;��ת����ת���, ������ת

Z_FAN:	
    MOV P0,R4
    LCALL DLS      ;������ʱ�ӳ���
    MOV A,P1
    RR A           ;����һλ
    MOV P1,A
    LCALL SCAN     ;����SCAN�ӳ���
    JMP Z_FAN      ;��ת����ת���, ������ת

SCAN:                 ;ɨ�跨��ȡ�����ӳ��򣬽�ȫ�������õ͵�ƽ��������ߵ�״̬��ֻҪ��һ�е�ƽΪ�ͣ����ʾ�������м�����
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

DLS:               ;��ʱ�ӳ���
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
