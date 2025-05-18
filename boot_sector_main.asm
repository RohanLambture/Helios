[org 0x7c00] ; global offset

mov bx ,msg ;store msg in bx 
call print

call newline

mov bx , bye 
call print

call newline

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