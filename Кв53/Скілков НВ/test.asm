Data1 Segment
Valb db 10101b
String db "String"
Valw dw 1355d
Vald dd 235df3ah
Dex db 13
Data1 ends
Data2 Segment
Val2d dd 35df23h
Data2 Ends
Assume cs:Code1,Ds:Data1,Gs:Data2
Code1 Segment
L1:
	sbb Valb[eax+edx],eax
	shl eax,1
	shl bl,1
	Jnc L2
	adc Val2d[eax+edx],eax
	sub eax,Vald[esi+edi+6]
	jmp Valw[eax+edx]
	not Valw[bx+di]
	not ds:Valw[esi+edi]
	not Valw[esi+edi+5]
	not String[bp+si]
	Jnc L1
	jmp L1
L2:
	nop
Code1 ends
Code2 Segment 
	sub ax,Valw[bx+di]
	adc ax,dx
	sub al,ah
Code2 ends
