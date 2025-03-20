mov ah, 0x0e ; tty mode

mov bp, 0x8000 ; this is an address far away from 0x7c00 so that we don't get overwritten
mov sp, bp ; if the stack is empty then sp points to bp

push 'A'
push 'B'
push 'C'

; this is will print 'A' 
mov al, [0x7ffe] ; 0x8000 - 2
int 0x10

; this is will print garbage value 
mov al, [0x8000]
int 0x10


pop bx  ; this is will pop the top element from that stack, and will put it in the bx register
mov al, bl ; as we put the top values in bx register, the value 'C'(8 bit) is stored in the lower part of bx ie bl
int 0x10 ; prints C

pop bx
mov al, bl
int 0x10 ; prints B

pop bx
mov al, bl
int 0x10 ; prints A

; data that has been pop'd from the stack is garbage now
mov al, [0x8000]
int 0x10

mov al, [0x7ffe] ; 0x8000 - 2
int 0x10

jmp $
times 510-($-$$) db 0
dw 0xaa55