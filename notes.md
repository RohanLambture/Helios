# Boot-Sector
 - BIOS doesn't know how to load the os , so it delegates the task to boot sector
 - Boot sector should be places in know standard location (the first sector of disk , which takes 512 bytes)
 - To check whether the **disk is bootable** or not , the BIOS check the 511 and 512th byte are 0x55AA (Master Boot Record)
 - Instead of Putting 0x55AA , we will be putting the 0xAA55 because x86 is little endian
 - `times 510-($-$$) db 0` - fill out '0' in all the bytes from current position to the 510th bytes
 - `dw 0xAA55` - write a 2 bytes word data at current position