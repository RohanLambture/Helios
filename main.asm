[org 0x7c00]
[bits 16]

mov ebx,msg
call print

jmp $

%include"print.asm"

msg:
	db "Hello world!",0


times 510-($-$$) db 0
dw 0xaa55