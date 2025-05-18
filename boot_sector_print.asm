
;Basic Logic travel till 0 (terminator)
print:
        pusha
        jmp helper

helper:
        mov al , [bx]
        cmp al , 0 
        je end

        mov ah , 0x0e
        mov al , [bx]
        int 0x10

        add bx , 1
        jmp helper

end:
        popa 
        ret 


newline:
        pusha

        mov ah,0x0e
        mov al, 0x0a ; newline character
        int 0x10

        popa
        ret 