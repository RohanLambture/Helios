; receiving the data in 'dx'
; For the examples we'll assume that we're called with dx=0x1234
print_hex:
	pusha
	
	mov cx, 0 ; our index variable

hex_loop:
	cmp cx, 4
	je end
	
	mov ax, dx
	and ax, 0x000f ; 0x1234 -> 0x0004 by masking first three to zeros
	add al, 0x30 ; add 0x30 to N to convert it to ASCII "N"
	cmp al, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F'
	jle step2
	add al, 7 ; 'A' is ASCII 65 instead of 58, so 65-58=7

step2:
	mov bx, HEX_OUT + 5
	sub bx, cx
	mov [bx], al
	ror dx, 4 ; 0x1234 -> 0x4123 -> 0x3412 -> 0x2341 -> 0x1234
	
	add cx, 1
	jmp hex_loop

end:
	mov bx, HEX_OUT
	call print
	
	popa
	ret

HEX_OUT:
	db '0x0000',0
