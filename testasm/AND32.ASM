						ASSUME	CS:CODE

						.386p
CODE					SEGMENT

						AND		AL,68H
						AND		EAX,01234567H
						AND		BL,77H
						AND		BYTE PTR [EDI+EDX*4+01234567H],86H
						AND		EBX,01234567H
						AND		DWORD PTR [ESI+EAX*2+01234567H],89ABCDEFH

						AND		ECX,0FFFFFF80H
						AND		DWORD PTR [EBP+77777777H],0FFFFFF80H

						AND		ECX,11H
						AND		DWORD PTR [ESP+77777777H],11H

						AND		BL,BH
						AND		CH,CL
						AND		EBX,ECX
						AND		ECX,EBX

						AND		BL,[EDI+11111111H]
						AND		EDX,[ESI+11111111H]

CODE					ENDS

						END
