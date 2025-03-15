# Notes 

# Boot-Sector
 - BIOS doesn't know how to load the os , so it delegates the task to boot sector
 - Boot sector should be places in know standard location (the first sector of disk , which takes 512 bytes)
 - To check whether the **disk is bootable** or not , the BIOS check the 511 and 512th byte are 0x55AA (Master Boot Record)
 - Instead of Putting 0x55AA , we will be putting the 0xAA55 because x86 is little endian
 - `times 510-($-$$) db 0` - fill out '0' in all the bytes from current position to the 510th bytes
 - `dw 0xAA55` - write a 2 bytes word data at current position
 
# TTY mode (Teletype output mode) 
 - this mode/function is used to print a single character on the screen and then advance the cursor to next position
 - Enabling the tty mode and printing the character
    
    ``` asm
        mov ah , 0x0e
        mov al , 'H'
        int 0x10
    ```
    
    - first we tell the bios that we want to use tty mode by putting the `0x0e` in `ah` (higher part of ax register)
    - then we store the character that we want to print in `al` (lower part of ax register)
    - finally we raise a interrup 0x10 which is general interrupt for video services