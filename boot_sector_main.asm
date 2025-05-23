[org 0x7c00]

;create a stack
mov bp,0x8000
mov sp ,bp

mov bx,0x9000 ;data buffer for storing the data from disks
mov dh ,2     ;number of sectors to read (sector 2, sector 3)
; the bios sets 'dl'

call disk_read

mov dx , [0x9000]

call print_hex
call newline

mov dx, [0x9000+512]

call print_hex
call newline

jmp $

%include"boot_sector_print.asm"
%include"boot_sector_disk.asm"


times 510-($-$$) db 0
dw 0xaa55

times 256 dw 0xdada
times 256 dw 0xabcd