mov ah,0x0e

mov al,[msg]
int 0x10

call newline
call carriage_return

mov bx,0x7c0
mov ds,bx

mov al,[msg]
int 0x10

call newline
call carriage_return

mov al,[es:msg]
int 0x10

call newline
call carriage_return

mov bx,0x7c0
mov es,bx
mov al,[es:msg]
int 0x10

jmp $

%include"boot_sector_print.asm"

msg:
	db "X"

times 510-($-$$) db 0
dw 0xaa55