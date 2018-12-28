ORG 0000H
	AJMP MAIN

ORG 0030H
MAIN:
    MOV SP, #60H   ;设置堆栈指针

LOOP:
    LCALL STATUS1  ;调用STATUS1
    LCALL DL10S    ;延时10s
    LCALL STATUS2  ;调用STATUS2
    LCALL DL2S     ;延时2s
    LCALL STATUS3  ;调用STATUS3
    LCALL DL10S    ;延时10s
    LCALL STATUS4  ;调用STATUS4
    LCALL DL2S     ;延时2s
    SJMP LOOP      ;继续循环

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

DL10S:             ;延时10s子程序
    MOV R1, #5     ;即调用5次延时2s子程序
DL5:
    LCALL DL2S
    DJNZ R1, DL5
    RET

DL2S:              ;三重循环延时2s子程序
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
