ORG 0000H
	AJMP MAIN

ORG 0030H
MAIN:
    MOV SP, #60H   ;���ö�ջָ��

LOOP:
    LCALL STATUS1  ;����STATUS1
    LCALL DL10S    ;��ʱ10s
    LCALL STATUS2  ;����STATUS2
    LCALL DL2S     ;��ʱ2s
    LCALL STATUS3  ;����STATUS3
    LCALL DL10S    ;��ʱ10s
    LCALL STATUS4  ;����STATUS4
    LCALL DL2S     ;��ʱ2s
    SJMP LOOP      ;����ѭ��

STATUS1:
    MOV A, #0F3H
	MOV DPTR, #U4
	MOVX @DPTR, A  ;@U4 = #0F3H
	MOV A, #0CH
	MOV DPTR, #U5
	MOVX @DPTR, A  ;@U5 = #0CH
	RET

STATUS2:
	MOV A, #0C3H
	MOV DPTR, #U4
	MOVX @DPTR, A  ;@U4 = #0C3H
    MOV A, #0FH
	MOV DPTR, #U5
	MOVX @DPTR, A  ;@U5 = #0FH
	RET

STATUS3:
    MOV A, #0FCH
	MOV DPTR, #U4
	MOVX @DPTR, A  ;@U4 = #0FCH
	MOV A, #03H
	MOV DPTR, #U5
	MOVX @DPTR, A  ;@U5 = #03H
	RET

STATUS4:
  	MOV A, #3CH
	MOV DPTR, #U4
	MOVX @DPTR, A  ;@U4 = #3CH
    MOV A, #0FH
	MOV DPTR, #U5
	MOVX @DPTR, A  ;@U5 = #0FH
	RET

DL10S:             ;��ʱ10s�ӳ���
    MOV R1, #5     ;������5����ʱ2s�ӳ���
DL5:
    LCALL DL2S
    DJNZ R1, DL5
    RET

DL2S:              ;����ѭ����ʱ2s�ӳ���
    MOV R7, #20    ;t = ((2 * 250) * 200) * 20 = 2,000,000 us = 2s
DL2:
    MOV R6, #200
DL1:
    MOV R5, #250
	DJNZ R5, $
	DJNZ R6, DL1
	DJNZ R7, DL2
	RET

U4 EQU 0FE00H      ;U4 = 0FE00H
U5 EQU 0FD00H      ;U5 = 0FD00H

END
