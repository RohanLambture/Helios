print_rm:
	pusha

print_rm_loop:
	mov al ,[bx]

	cmp al,0
	je print_rm_end

	mov ah,0x0e
	int 0x10

	add bx,1
	jmp print_rm_loop

print_rm_end:
	popa
	ret


print_rm_newline:
	pusha

	mov ah, 0x0e
	mov al, 0x0a ; newline char
	int 0x10

	mov al, 0x0d ; carriage return
	int 0x10

	popa
	ret