[org 0x7c00] ; global offset

mov bx ,msg ;store msg in bx 
call print

call newline
call carriage_return

mov bx , bye 
call print

call newline
call carriage_return

mov dx,0x1234
call print_hex


jmp $

msg:
        db "Rohan",0
bye:
        db "Bye!",0

;include other assembly files
%include"boot_sector_print.asm"

;MBR or magic number 
times 510-($-$$) db 0
dw 0xaa55