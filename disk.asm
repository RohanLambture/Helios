; read dh sectors from dl drive

disk_load:
	pusha
	push dx ; push the dx register onto the stack as we are going to changes it contents

	mov ah,0x02 ; select read from disk function
	mov al,dh

	mov dh,0x00 ; hardcoded to head number to 0
	; dl is already set by bios

	mov ch ,0x00 ; cylinder number =0
	mov cl,0x02  ; starting sector form where to start reading

	;[es:bx] is where want to store the data
	int 0x13
	jc disk_error; if carry flag is set , then error

	pop dx
	cmp al,dh ;bios set the number for sector read in the al
	jne sector_error
	jmp disk_end

disk_end:
	popa
	ret

disk_error:
	mov bx,disk_error_msg
	call print_rm
	call print_rm_newline
	jmp disk_end

sector_error:
	mov bx, sector_error_msg
	call print_rm
	call print_rm_newline
	jmp disk_end


disk_error_msg:
	db "Error while reading",0
sector_error_msg:
	db "Incorrect no of sectors read" , 0