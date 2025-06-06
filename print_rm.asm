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

; we expect input as format of '0xabab' in dx register
print_rm_hex:
	pusha
	mov cx , 0 ; counter

print_rm_hex_loop:
	cmp cx,4
	je print_rm_hex_end

	mov ax , dx
	and ax,0x000f ; ax will only have last digit
	add al ,0x30

	cmp al,0x39
	jle print_rm_hex_help
	add al,7

print_rm_hex_help:
	mov bx,HEX_MSG+5 ; position in msg to put current character
	sub bx,cx
	mov [bx],al
	ror dx, 4

	add cx ,1
	jmp print_rm_hex_loop

print_rm_hex_end:
	mov bx,HEX_MSG
	call print_rm

	popa
	ret

HEX_MSG:
	db "0x0000", 0
