ORG 0000H
    AJMP MAIN

ORG 0030H
MAIN:
    MOV SP, #60H     ;���ö�ջָ��
    MOV P2, #0FEH    ;��P2��P2.0��ȫ��Ϊ�ߵ�ƽ
LOOP1:
    JB P1.0, LOOP1   ;P1.0λΪ0�������ִ��, �����ڴ����ѭ��
    MOV A, P2        ;��P2��ֵ����A
    RL A             ;��Aѭ������
    MOV P2,A         ;��A��ֵ����P2
    ACALL DL100ms    ;������ʱ100ms�ӳ���
    SJMP LOOP1       ;����ѭ��

DL100ms:             ;˫��ѭ����ʱ100ms�ӳ���
    MOV R7, #200     ;t = (2 * 250) * 200 = 100000us = 100ms
DL: MOV R6, #250
    DJNZ R6, $
    DJNZ R7, DL
    RET
END
