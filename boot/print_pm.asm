[bits 32] ;using 32-bit protected mode

;32-bit protected mode print
print_pm:
	pusha
	mov ecx , 0xb8000 ;start of VGA-memory
	mov edx ,0x0f     ; color

;taking characters from ebx and filling the VGA-memory
print_pm_loop:
	mov al,[ebx]
	mov ah,dl

	cmp al,0
	je print_pm_end


	mov [ecx],ax
	add ecx,2
	add ebx,1

	jmp print_pm_loop

print_pm_end:
	popa
	ret
