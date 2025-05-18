
;Basic Logic travel till 0 (terminator)
print:
        pusha
        jmp print_helper

print_helper:
        mov al , [bx]
        cmp al , 0 
        je print_end

        mov ah , 0x0e
        mov al , [bx]
        int 0x10

        add bx , 1
        jmp print_helper

print_end:
        popa 
        ret 


newline:
        pusha

        mov ah,0x0e
        mov al, 0x0a ; newline character
        int 0x10

        popa
        ret 

carriage_return:
        pusha

        mov ah,0x0e
        mov al,0x0d
        int 0x10

        popa
        ret

;considering that the dx will have the hex value 
; eg dx = 0x1234
print_hex:
        pusha

        mov cx ,0 ;index
         
print_hex_loop:
        cmp cx, 4
        je print_hex_end

        ;convert hex to ascii for 0-9 have to add 30 and for A-F add 41 
        mov ax,dx
        and ax,0x000f
        add al,0x30        

        cmp al,0x39 ; cmp whether <9 or  not 
        jle print_hex_helper        
        add al,7 ; means it was A-F character 

print_hex_helper:
        mov bx, hex_message
        add bx,5
        sub bx,cx


        mov [bx],al
        ror dx,4 ;to shift the last digit to right

        add cx,1
        jmp print_hex_loop

print_hex_end:
        mov bx, hex_message
        call print

        call newline
        call carriage_return

        popa
        ret
        

hex_message:
        db "0x0000",0 