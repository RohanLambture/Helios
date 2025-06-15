gdt_start:
	; the GDT starts with a null 8-byte
	dd 0x0 ; 4 byte
	dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
gdt_code: 
	dw 0xffff
	dw 0x0
	db 0x0
	db 10011010b
	db 11001111b
	db 0x0 

; GDT for data segment. base and length identical to code segment
gdt_data:
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0

gdt_end:

; GDT descriptor
gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start 

;some constants 
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
