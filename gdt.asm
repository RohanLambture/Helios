gdt_start:

;the first entry - null descriptor
gdt_null:
	dd 0x0
	dd 0x0

;the code segment descriptor
gdt_code:
	dw 0xffff ; Limit (bits 0-15)
	dw 0x0 ; Base (bits 0-15)
	db 0x0 ; Base (bits 16 -23)
	db 10011010b ; 1st flags , type flags
	db 11001111b ; 2nd flags , Limit (bits 16-19)
	db 0x0 ; Base (bits 24 -31)

;the data segment descriptor
gdt_data:
	dw 0xffff ; Limit (bits 0-15)
	dw 0x0 ; Base (bits 0-15)
	db 0x0 ; Base (bits 16 -23)
	db 10010010b ; 1st flags , type flags
	db 11001111b ; 2nd flags , Limit (bits 16-19)
	db 0x0 ; Base (bits 24 -31)

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; Size of our GDT
	dd gdt_start ; Start address of our GDT

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start