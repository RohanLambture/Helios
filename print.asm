[bits 32]

print:
	pusha
	mov ecx , 0xb8000
	mov edx ,0x0f

print_helper:
	mov al,[ebx]
	mov ah,dl

	cmp al,0
	je print_end


	mov [ecx],ax
	add ecx,2
	add ebx,1

	jmp print_helper

print_end:
	popa
	ret