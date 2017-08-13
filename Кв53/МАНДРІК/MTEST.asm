data segment
cb1 db 00013h
sb1 db "String"
cw2 dw 001b5h
cd3 dd 0005fh
const equ 00001h
sconst equ "const"
adeb equ cd3[ebx*2]
data ends
code segment 
l1:
	if const
		inc edx
	else
		mov ecx, cb1[eax*4]
	endif
	mov edi, adeb
	inc eax
	jnz l2
	cmp eax,ecx
	and es:cd3[ebx*2],ebx
	and adeb,eax
	shr eax, 000ffh	
	jnz l1	
	stos es:sb1[esi*2]
	add cw2[edi*2], 2h
	add al bl
l2:movsb
code ends
end