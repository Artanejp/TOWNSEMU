						.186

						ORG		0
						ASSUME	CS:DATA_CODE,DS:DATA_CODE,SS:STACK


DATA_CODE				SEGMENT



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



MAIN					PROC

						PUSH	ES
						PUSH	DS
						PUSHA

						PUSH	CS
						POP		DS
						PUSH	CS
						POP		ES



						MOV		DX,480H
						IN		AL,DX
						XOR		AH,AH
						CALL	PRINT_AX
						CALL	PRINT_CRLF



						MOV		DX,480H
						IN		AL,DX
						OR		AL,1
						OUT		DX,AL

						IN		AL,DX
						XOR		AH,AH
						CALL	PRINT_AX
						CALL	PRINT_CRLF



						MOV		DX,480H
						IN		AL,DX
						AND		AL,0FEH
						OUT		DX,AL

						IN		AL,DX
						XOR		AH,AH
						CALL	PRINT_AX
						CALL	PRINT_CRLF



						MOV		DX,480H
						IN		AL,DX
						OR		AL,1
						OUT		DX,AL

						IN		AL,DX
						XOR		AH,AH
						CALL	PRINT_AX
						CALL	PRINT_CRLF



						POPA
						POP		DS
						POP		ES

EXIT:
						; Termination
						MOV	AH,4CH
						MOV	AL,00H
						INT	21H


MAIN					ENDP



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MSG_REG_AX				DB		"AX=",0
MSG_REG_BX				DB		" BX=",0
MSG_REG_CX				DB		" CX=",0
MSG_REG_DX				DB		" DX=",0
MSG_REG_SI				DB		" SI=",0
MSG_REG_DI				DB		" DI=",0
MSG_REG_CS				DB		" CS=",0
MSG_REG_DS				DB		" DS=",0
MSG_REG_ES				DB		" ES=",0


PRINT_REGISTER			PROC

						PUSHA
						PUSH	DI

						LEA		DI,[MSG_REG_AX]
						CALL	PRINT_CSTR
						CALL	PRINT_AX

						MOV		AX,BX
						LEA		DI,[MSG_REG_BX]
						CALL	PRINT_CSTR
						CALL	PRINT_AX

						MOV		AX,CX
						LEA		DI,[MSG_REG_CX]
						CALL	PRINT_CSTR
						CALL	PRINT_AX

						MOV		AX,DX
						LEA		DI,[MSG_REG_DX]
						CALL	PRINT_CSTR
						CALL	PRINT_AX

						MOV		AX,SI
						LEA		DI,[MSG_REG_SI]
						CALL	PRINT_CSTR
						CALL	PRINT_AX

						POP		DI

						MOV		AX,DI
						LEA		DI,[MSG_REG_DI]
						CALL	PRINT_CSTR
						CALL	PRINT_AX

						MOV		AX,CS
						LEA		DI,[MSG_REG_CS]
						CALL	PRINT_CSTR
						CALL	PRINT_AX

						MOV		AX,DS
						LEA		DI,[MSG_REG_DS]
						CALL	PRINT_CSTR
						CALL	PRINT_AX

						MOV		AX,ES
						LEA		DI,[MSG_REG_ES]
						CALL	PRINT_CSTR
						CALL	PRINT_AX

						MOV		AL,'$'
						CALL	PUTC

						POPA

						RET

PRINT_REGISTER			ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PRINT_AL				PROC
						PUSH	CX
						PUSH	AX

						CALL	ITOA8
						XCHG	AL,AH
						CALL	PUTC
						XCHG	AL,AH
						CALL	PUTC

						POP		AX
						POP		CX
						RET
PRINT_AL				ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PRINT_AX				PROC
						XCHG	AL,AH
						CALL	PRINT_AL
						XCHG	AL,AH
						CALL	PRINT_AL
						RET
PRINT_AX				ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PRINT_AX_CH				PROC

						; FM TOWNS BIOS disassembly strongly suggests that CH is the error code from INT 93H AH=0FDH.
						; The meaning of CH is still unknown, but 0 means no error.
						PUSH	CX
						PUSH	AX
						PUSH	BP

						MOV		BP,SP
						MOV		AL,BYTE PTR [BP+3]
						CALL	PRINT_AL
						MOV		AL,BYTE PTR [BP+2]
						CALL	PRINT_AL

						MOV		AL,' '
						CALL	PUTC
						MOV		AL,BYTE PTR [BP+5]
						CALL	PRINT_AL

						MOV		AL,' '
						CALL	PUTC

						POP		BP
						POP		AX
						POP		CX

						RET

PRINT_AX_CH				ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PRINT_AH_CX				PROC

						; CD-ROM BIOS Error Code
						PUSH	CX
						PUSH	AX
						PUSH	BP

						MOV		BP,SP
						MOV		AL,BYTE PTR [BP+3]
						CALL	PRINT_AL

						MOV		AL,' '
						CALL	PUTC

						MOV		AL,BYTE PTR [BP+5]
						CALL	PRINT_AL
						MOV		AL,BYTE PTR [BP+4]
						CALL	PRINT_AL

						MOV		AL,' '
						CALL	PUTC

						POP		BP
						POP		AX
						POP		CX

						RET

PRINT_AH_CX				ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PRINT_CRLF				PROC
						PUSH	AX
						MOV		AL,0DH
						CALL	PUTC
						MOV		AL,0AH
						CALL	PUTC
						POP		AX
						RET
PRINT_CRLF				ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PRINT_CSTR				PROC
						PUSH	DX
						PUSH	CX
						PUSH	AX
@@:
						MOV		DL,CS:[DI]
						CMP		DL,0
						JE		PRINT_CSTR_EXIT
						MOV		AH,06H
						INT		21H
						LEA		DI,[DI+1]
						JMP		@b

PRINT_CSTR_EXIT:
						POP		AX
						POP		CX
						POP		DX
						RET

PRINT_CSTR				ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUTC					PROC

						PUSH	AX
						PUSH	DX

						MOV		DL,AL
						MOV		AH,06H
						INT		21H

						POP		DX
						POP		AX

						RET

PUTC					ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Input
;	AL		Number
; Output
;	AH		Higher digit ASCII code
;	AL		Lower digit ASCII code
ITOA8					PROC

						MOV		AH,AL
						AND		AL,0FH
						SHR		AH,1
						SHR		AH,1
						SHR		AH,1
						SHR		AH,1

						ADD		AL,'0'
						CMP		AL,'9'
						JLE		@f
						ADD		AL,'A'-'0'-10
@@:
						ADD		AH,'0'
						CMP		AH,'9'
						JLE		@f
						ADD		AH,'A'-'0'-10
@@:
						RET

ITOA8					ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



DATA_CODE				ENDS

STACK					SEGMENT	STACK

						DB	1024 DUP(0)

STACK					ENDS

						END		MAIN


