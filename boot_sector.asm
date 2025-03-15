mov ah , 0x0e ; to use the tty mode
mov al , 'H'
int 0x10 ; raises the 0x10 interrupt
mov al , 'e' 
int 0x10
mov al , 'l'
int 0x10
int 0x10 
mov al ,'o'
int 0x10

jmp $ ; jump to current address infinitely

times 510-($-$$) db 0
dw 0xaa55