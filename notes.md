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

    ``` nasm
        mov ah , 0x0e
        mov al , 'H'
        int 0x10
    ```

    - first we tell the bios that we want to use tty mode by putting the `0x0e` in `ah` (higher part of ax register)
    - then we store the character that we want to print in `al` (lower part of ax register)
    - finally we raise a interrupt 0x10 which is general interrupt for video services

# Boot-Sector memory
  - BIOS loads the Boot-Sector at `0x7C00` and transfers the control to 0x7C00, from where the bootloader code starts running (see the image below)
  - `org 0x7c00` - assemblers defines a **global offset** for every memory location , therefore all the memory address are calculated relative to `0x7c00`
![Alt text](/images/memory_layout.png)


> # Boot Process
>   - On powering on the computer, the system's firmware is activated. (Firmware- BIOS,EFI,UEFI)
>   - Firmware runs the `POST (Power-on self test)` which is diagnoses the hardware.
>   - After that , firmware looks for bootable device, which is usually placed at `Cylinder 0, Head 0 , sector 1/0` depending upon CHS / LBA addaressing mode respectively.
>   - Firmware reads boot-sector from bootable device and loads it 512 bytes in RAM at address `0x7C00`
>   - Transfer the control to `0x7C00` which is `boot-loader`
>   - Boot-loader does further loading such loading a larger bootloader , switch cpu from real mode to protected mode etc


# Boot-Sector stack
  - `bp` register stores the base address (bottom) of stack ,and `sp` stores the top.
  - As x86 is little endian, the stack grows downward from `bp` (i.e `sp` gets decreased)
  - Setting up the stack
    ```nasm
      mov bp, 0x8000
      mov sp ,bp
    ```
    - `mov bp , 0x8000` sets 0x8000 in bp (i.e. the the base address of stack)
    - ` mov sp , bp` as initially the stack it empty so , bottom and top address is same
    ```nasm
      push 'A'
      pop bx
      mov al ,bl
      int 0x10 ; print A
    ```
    - `push 'A'` will push A into stack , which will decrement the value sp , the new ***sp value =0x8000-2= 0x7ffe***
    - `pop bx` this is will pop the top element from that stack, and will put it in the `bx` register
    - ` mov al,bl` as we put the top values in `bx` register, the value 'A'(8 bit) is stored in the lower part of `bx` ie `bl`

> # General-Purpose registers in x86 assembly
>   - `EAX` ,`EBX`,`ECX`,`EDX` are considers **general-purpose registers**
>   - Each 32-bit registers can be broken down into a lower 16-bit portion
>       - `AX(Accumulator Register)` = lower 16 bits of `EAX`
>       - `BX(Base Register)` = lower 16 bits of `EBX`
>       - `CX(Count Register)` = lower 16 bits of `ECX`
>       - `DX(Data Register)` = lower 16 bits of `EDX`
>
>  ![Alt text](/images/registers.jpg)

# Function Calling & Printing
  - `call` vs `jmp` : if you want to use `ret` to return to parent function use `call`.
  - `db 'msg', 0` : for creating a variable a `string = "msg"` terminated by null character.
    > Note : "msg" and 'msg' are interchangeable
  - `%include "file.asm"`: assembler’s preprocessor reads contents of file and insert it in place as if that file’s source lines were written directly in the current assembly file
  - `pusha` and `popa` are used to used to save and restore the full set of general-purpose registers onto and from the stack, respectively
  - `0x0a` is for newline character.
  - `ror dx,4` : rotates the `dx` register to right by 4 bits

# Segmenation
  - Real Mode
    - Logical address is in the form `segment:offset` . This is traslated to physical address using equation `Physical address = (segment << 4 ) + offset `

  - Protected Mode
    - Logical address is `segment_selector:offset`.
    - A `segment_selector` represents an offset into a system table called the `Global Descriptor Table (GDT)`. The GDT contains a list of descriptors.Each of these contains the info about segment. Few of the fields are:
      - `base address` of segment
      - `limit` of segment
      - `privilege level (rings)` of descriptor
    > Note: This is not actual physical address if paging is enabled .This helps in flat model
    - Given logical address, `Physical address = segment base (found from GDT[segment_selector]) + offset `

 > Note: Two different segments can overlap.

  -  `Segmentation Registers `: x86 has 6 segment registers

		|Segment Register   |Purpose   |
		|---|---|
		|CS (Code Segment)   |Code execution eg jmp/call         |
		|DS (Data Segment)   |Data access                        |
		|SS (Stack Segment)  |Stack operations eg push,pop       |
		|ES (Extra Segment)  |String destinations eg movs,cmps   |
		|FS & GS (General)   |General-purpose    		 |

     - `mov al,[msg]` won't work without applying offset 0x7c0 (where the bootloader is loaded)
     - `[msg]` is just offset from 0x7c0
     ```nasm
     mov bx,0x7c0
     mov ds, bx
     ```
     -  Because of this all memory references will be offset by 'ds'
     - ` mov al,[es:msg]`:es is used as base for memory access and `es` is also starts at 0 therefore won't work

# Logical Address vs Linear Address
  - ` Logical address = (segment_selector:offset)`
	- `mov ax,[msg]` for this instruction , `[msg]` is just offset with base `ds`.So `ax = ds:msg`
	- `ax = ds:msg` is the Logical address
  - `Linear address = descriptor_base+offset`
	- In protected mode, each segment selector(eg `ds`) has an entry in GDT , which contains the base and limit .If `offset <=limit` then cpu computes linear address
	- In `flat memory` , OS arranges each segment selector has `base = 0` and `limit = 0xFF...(upper limit)` hence `linear address = offset `.So logical address's offset is linear address directly.

> # Flat-Memory model
>   - Most C compilers assume flat-memory model
>   - In this model, all segments points to the same base ie 0 , have limit of 4GB for 32 bit-mode.
>   - Note that this is not actual physical address but logical address/linear address only which then converted by MMU(Memory Mangement Unit).This ensures isolation between processes.

# Disk access using the BIOS (INT 13h)