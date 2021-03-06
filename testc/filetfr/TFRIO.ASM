						.386p
						ASSUME	CS:CODE

						PUBLIC	VMFILE_READ_BATCH



SPRITERAM_SEGMENT				EQU		0114H
TOWNS_VMIF_CMD_FILE_RXRDY		EQU 	04H
TOWNSIO_VM_HOST_IF_CMD_STATUS	EQU		2386H



CODE					SEGMENT

; void VMFILE_READ_BATCH(unsigned char buf[128*1024]);
VMFILE_READ_BATCH		PROC
						PUSH	EBP				; [EBP]=PrevEBP,  [EBP+4]=EIP,  [EIP+8]=BufferPtr
						MOV		EBP,ESP
						PUSHAD
						PUSHFD
						PUSH	ES

						MOV		AX,SPRITERAM_SEGMENT
						MOV		ES,AX

						CLI
						MOV		ESI,0
						MOV		EDI,[EBP+8]
						MOV		ECX,32*1024
SPRITERAM_SAVE_LOOP:	MOV		EAX,ES:[ESI]
						MOV		[EDI],EAX
						LEA		ESI,[ESI+4]
						LEA		EDI,[EDI+4]
						LOOP	SPRITERAM_SAVE_LOOP



						MOV		AL,TOWNS_VMIF_CMD_FILE_RXRDY
						MOV		DX,TOWNSIO_VM_HOST_IF_CMD_STATUS
						OUT		DX,AL



						MOV		ESI,0
						MOV		EDI,[EBP+8]
						MOV		ECX,32*1024
SPRITERAM_XCHG_LOOP:	MOV		EAX,ES:[ESI]
						MOV		EBX,[EDI]
						MOV		[EDI],EAX
						MOV		ES:[ESI],EBX
						LEA		ESI,[ESI+4]
						LEA		EDI,[EDI+4]
						LOOP	SPRITERAM_XCHG_LOOP



						POP		ES
						POPFD
						POPAD
						POP		EBP
						RET
VMFILE_READ_BATCH		ENDP



CODE					ENDS
						END
