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
>   - After that , firmware looks for bootable device, which is usually placed at `Cylinder 0, Head 0 , sector 1/0` depending upon CHS / LBA addressing mode respectively.
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
    - `mov bp , 0x8000` sets 0x8000 in bp (i.e. the base address of stack)
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

# Segmentation
  - Real Mode
    - Logical address is in the form `segment:offset` . This is translated to physical address using equation `Physical address = (segment << 4 ) + offset `

  - Protected Mode
    - Logical address is `segment_selector:offset`.
    - A `segment_selector` represents an offset into a system table called the `Global Descriptor Table (GDT)`. The GDT contains a list of descriptors.Each of these contains the info about segment. Few of the fields are:
      > Note :GDT is specific to x86-64 arch.
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

# Logical Address vs Linear Address vs Physical address vs Virtual address
  - `Virtual address`
	- The address seen by programm
	- **Virtual address** is the `offset within segment`
	- In **Segmented Mode** , `Linear address = Segment Base + virtual address (offset)`
	- In **Flat Memory Model**, `Linear address = virtual address = offset`
  - ` Logical address`
	- The address as seen by the CPU
	- `segment_selector:offset` in protected mode and `segment:offset` in real mode
	- eg. `mov ax,[ds:msg]` - here is `ds:msg` is the logical address
  - `Linear address`
	- The adderss after segmentation but before paging
	- **Real Mode** :`Physical address = (segment << 4)+ offset`
	- **protected mode** :`Linear address = descriptor_base+offset`
	-  each segment selector(eg `ds`) has an entry in GDT , which contains the base and limit .If `offset <=limit` then cpu computes linear address

	- In `flat memory` , OS arranges each segment selector has `base = 0` and `limit = 0xFF...(upper limit)` hence `linear address = offset `.So logical address's offset is linear address directly.
  - `Physical address`
	- The actual address on the memory , final result after all translation (**segmentation + paging**)

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│  Program Code:  mov eax, [0x08048000]                                                                                                                        │
└──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
                                                                                    │
                                                                                    ▼
┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                                    MEMORY MODEL COMPARISON                                                                                   │
│                                                                                                                                                              │
│  ┌─────────────────────────────────────────────────────────────────────────┐     ┌─────────────────────────────────────────────────────────────────────────┐ │
│  │                    FLAT MEMORY MODEL                                    │     │                   SEGMENTED MEMORY MODEL                                │ │
│  │                   (Modern x86-64/x86-32)                                │     │                    (Legacy x86-16)                                      │ │
│  │                                                                         │     │                                                                         │ │
│  │              NO LOGICAL ADDRESS                                         │     │               HAS LOGICAL ADDRESS                                       │ │
│  └─────────────────────────────────────────────────────────────────────────┘     └─────────────────────────────────────────────────────────────────────────┘ │
│                                                                                                                                                              │
│  ┌─────────────────────┐     ┌─────────────────────┐     ┌─────────────────┐     ┌─────────────────────┐     ┌─────────────────────┐     ┌─────────────────┐ │
│  │   VIRTUAL ADDRESS   │ →   │   LINEAR ADDRESS    │ →   │ PHYSICAL ADDRESS│     │   LOGICAL ADDRESS   │ →   │   LINEAR ADDRESS    │ →   │ PHYSICAL ADDRESS│ │
│  │  (What program uses)│     │ (After segmentation)│     │ (After paging)  │     │ (segment:offset)    │     │ (After segmentation)│     │ (After paging)  │ │
│  │                     │     │                     │     │                 │     │                     │     │                     │     │                 │ │
│  │    0x08048000       │     │    0x08048000       │     │   0x12345678    │     │   DS:0x8000         │     │    0x08048000       │     │   0x12345678    │ │
│  │                     │     │ (same as virtual)   │     │ (actual RAM)    │     │ (16-bit segment:    │     │ DS*16 + 0x8000      │     │ (actual RAM)    │ │
│  │ Process Virtual     │     │ Identity mapping    │     │                 │     │  16-bit offset)     │     │ = 0x08048000        │     │ MMU translated  │ │
│  │ Memory Space        │     │ (base = 0x00000000) │     │                 │     │                     │     │                     │     │                 │ │
│  └─────────────────────┘     └─────────────────────┘     └─────────────────┘     └─────────────────────┘     └─────────────────────┘     └─────────────────┘ │
│                                                                                                                                                              │
│  Key Differences:                                                                                                                                            │
│  • Flat Model: NO logical addresses - programs work directly with virtual addresses                                                                          │
│  • Segmented Model: Programs use logical addresses (segment:offset pairs)                                                                                    │
│  • Flat Model: Virtual Address = Linear Address (segmentation is transparent)                                                                                │
│  • Segmented Model: Logical Address → Linear Address → Physical Address                                                                                      │
└──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
```


> # Flat-Memory model
>   - Most C compilers assume flat-memory model
>   - In this model, all segments points to the same base ie 0 , have limit of 4GB for 32 bit-mode.
>   - Note that this is not actual physical address but logical address/linear address only which then converted by MMU(Memory Management Unit).This ensures isolation between processes.

# Disk access using the BIOS (INT 13h)
  - INT 0x13 is a BIOS interrupt service that provides disk I/O operations.
  - `Input registers`
    - `AH`: To select the function needed eg for `Read` AH =`0x02`, for `write` AH= `0x03`
    - `AL`: Number of sectors to read
    > Note: Cylinder Number is 10 bit long
    - `CH`: Contains the first 8 bits of cylinder number (bits 0-7)
    - `CL`: Contains sector number (bits 0-5) and Remaining two bits of cylinder number
    - `DH`: Head Number
    - `DL`: Drive Number set by bios itself (eg `0x00` if booted from `floppy` , `0x80` if booted from `first hard disk` etc )
    - `ES:BX`:Buffer, memory address to store data

  - `Output registers`
    - `Carry flag`: 0 for successful and 1 if error
    >
    >```nasm
    > int 0x13 ;calls interrupt
    > jc disk_error ; check if carry flag is 1, if yes then error
    >```
    - `AH`:status code (0=success ,error code if failed)
    - `AL`: number of sectors actually read

  - Example Code
  ```nasm
  [org 0x7c00]
  ; setup the stack
  mov bp, 0x8000
  mov sp, bp

  ; set up data buffer es:bx= 0x9000
  mov bx, 0x9000
  mov dh, 2

  call disk_load

  mov dx, [0x9000]
  mov dx, [0x9000 + 512]


  disk_load:
	pusha
	push dx

	;set all the registers

	int 0x13
	jc disk_error

	cmp dh,al
	jc disk_error

	pop
	ret
  ```
  - `Memory Map`
```text
	0x7c00 ├─────────────┐
           │ Bootloader  │ 512 bytes
	0x7dff ├─────────────┤
	       │ Free space  │
	       │     ↑       │
	       │   Stack     │ Stack grows downward
	0x8000 ├─────────────┤ ← SP (Stack Pointer)
	       │ Free space  │
	0x9000 ├─────────────|
	       │ Data Buffer │ 1024 bytes (2 sectors)
	0x93ff └─────────────┘
```
  - Converting LBA to CHS
    -  LBA = a zero-based sector index (0…n–1)
    -  SPT = “sectors per track” (typical BIOS limits: 63)
    -  HPC = “heads per cylinder” (typical BIOS limits: 16)
    - Compute:
      - temp = LBA / SPT (integer division)
      - Sector = (LBA % SPT) + 1
      - Head = temp % HPC
      - Cylinder = temp / HPC
> Note: Sector numbers on CHS start at 1; head and cylinder start at 0 and for LBA starts at 0.

# VGA (Video Graphics Array)
  - In **32-bit Protected Mode** , We lost BIOS interrupts and we'll need to code the GDT
  - To print something we use `VGA-Memory`,it has **text mode** instead of pixels manipulation
  - The VGA-Memory starts at `0xb8000` and has size of `4000 bytes` .Each character has `2 bytes`( one for ascii and other for color and such)
  - The formula for accessing a specific character on **80x25** grid is
    `0xb8000 + 2 * (row * 80 + col)`

# Global Descriptor Table (GDT)
> Note : Previous info at [Segmentation Section](#segmentation)

![Alt text](/images/segment_descriptor.png)
  - `Defininig the GDT`
    - The first entry should be **null entry**
    - In flat memory model, `base=0x0` and `limit=0xfffff`

# GDT Entry Structure Table

| Field | Location | Bits | Description |
|-------|----------|------|-------------|
| **Limit (Low)** | Bytes 0-1 | 16 bits | Lower 16 bits of segment limit |
| **Base (Low)** | Bytes 2-3 | 16 bits | Lower 16 bits of base address |
| **Base (Middle)** | Byte 4 | 8 bits | Middle 8 bits of base address (bits 16-23) |
| **Access Byte** | Byte 5 | 8 bits | Access rights and type information |
| **Limit (High) + Flags** | Byte 6 | 8 bits | Upper 4 bits of limit + 4 flag bits |
| **Base (High)** | Byte 7 | 8 bits | Upper 8 bits of base address (bits 24-31) |

## Access Byte (Byte 5) Breakdown

| Bit | Name | Values | Description |
|-----|------|--------|-------------|
| **7** | **P** (Present) | 0 = Not present<br>1 = Present | Segment presence in memory |
| **6-5** | **DPL** (Descriptor Privilege Level) | 00 = Ring 0 (Kernel)<br>01 = Ring 1<br>10 = Ring 2<br>11 = Ring 3 (User) | Required privilege level |
| **4** | **S** (Descriptor Type) | 0 = System descriptor<br>1 = Code/Data segment | Descriptor category |
| **3** | **E** (Executable) | 0 = Data segment<br>1 = Code segment | Segment type |
| **2** | **DC** (Direction/Conforming) | **Data:** 0 = Grow up, 1 = Grow down<br>**Code:** 0 = Non-conforming, 1 = Conforming | Growth direction or conforming behavior |
| **1** | **RW** (Read/Write) | **Data:** 0 = Read-only, 1 = Read/Write<br>**Code:** 0 = Execute-only, 1 = Execute/Read | Access permissions |
| **0** | **A** (Accessed) | 0 = Not accessed<br>1 = Accessed | Hardware-set access flag |

## Flags (Byte 6, bits 4-7) Breakdown

| Bit | Name | Values | Description |
|-----|------|--------|-------------|
| **7** | **G** (Granularity) | 0 = Byte granularity (max 1MB)<br>1 = 4KB page granularity (max 4GB) | Limit scaling factor |
| **6** | **D/B** (Default/Big) | **Code:** 0 = 16-bit, 1 = 32-bit<br>**Data:** 0 = 16-bit limit, 1 = 32-bit limit<br>**Stack:** 0 = SP, 1 = ESP | Default operation size |
| **5** | **L** (Long Mode) | 0 = Not 64-bit<br>1 = 64-bit code segment | 64-bit mode indicator |
| **4** | **AVL** (Available) | 0 or 1 (OS-defined) | Available for system software use |


# Switch to 32-bit Protected Mode
  - To jump/switch to Protected Mode
    - **Disable interrupts**: ` cli ` will disable the interrupts
    - **Load our GDT**: `    lgdt [gdt_descriptor] `
    - Set a bit on the CPU control register cr0
      ```nasm
      mov eax, cr0
      or eax, 0x1 ; 3. set 32-bit mode bit in cr0
      mov cr0, eax
      ```
      > Note: x86 CPUs only support mov instructions for control registers (CR0, CR3, CR4). Direct bit manipulation like or cr0, 0x1 doesn't exist.
    - Flush the CPU pipeline by far jump
      - `jmp CODE_SEG:init_pm`: this performs far jump to load CS register with proper PM32 descriptor
      > Note: If we perform near jump `jmp init_pm` CS regsiter will point to old value that is `0x7c00`
    - Update all the segment registers
      ```nasm
      [bits 32]
      init_pm:
          ; update the segment registers
          mov ax, DATA_SEG
          mov ds, ax
          mov ss, ax
          mov es, ax
          mov fs, ax
          mov gs, ax
      ```
      - `[bits 32]` tells the assembler to use `32-bit mode` from now on
      - Here, We are pointing every segment register to same descriptor because we are using flat memory model.
      - We are using the **DATA_SEG to update the registers** while we used the **CODE_SEG for updating the CS register** because of the `permissions differences`.

    >Note: In flat memory model, all segment registers (CS, DS, SS, ES, FS, GS) point to the same 4GB address space but have different access permissions. CS has executable+readable permissions for instruction fetching, while DS/SS/ES/FS/GS have readable+writable (non-executable) permissions for data access.

    - Update the stack
      - After switching to protected mode, **ESP and EBP still contain old values** from real mode
      ```nasm
      mov ebp, 0x90000
      mov esp, ebp
      ```
    - Call to a well-known label which contains the first useful code in 32 bits.Here `call BEGIN_PM`.

# Compilation Process
  - Compile C to Object File
    ```bash
    i386-elf-gcc -ffreestanding -c code.c -o code.o
    ```
    - `ffreestanding` flag: No standard library (kernel/bare metal)
    - `-c` flag : Compile only, don't link
    - Creates object file with debugging info and symbols

  - Examine Generated Assembly
    ```bash
    i386-elf-objdump -d code.o
    ```
    - Shows the assembly code generated by the compiler.

  - Link to Binary
    ```bash
    i386-elf-ld -o code.bin -Ttext 0x0 --oformat binary code.o
    ```

    - `Ttext 0x0`: Place code at memory address 0x0
    - `--oformat binary`: Pure machine code (no metadata)
    - Creates executable binary file

  - Analysis Tools
    ```bash
    xxd code.o
    xxd code.bin
    ```

  - Disassemble Machine Code
    ```bash
    ndisasm -b 32 code.bin
    ```

# Cursor Position using VGA card ports
  - `0x3d4` **VGA control register** - sets which VGA register to access
  - `0x3d5` **VGA data register** - reads/writes the selected VGA register
  ```text 
			  VGA Controller Internal Layout:
			┌─────────────────────────────────────┐
			│  VGA Chip Internal Registers        │
			│  ┌─────┬─────┬─────┬─────┬─────┐    │
			│  │ #0  │ #1  │ #2  │ ... │ #24 │    │
			│  └─────┴─────┴─────┴─────┴─────┘    │
			│         ▲                           │
			│         │                           │
			│  ┌─────────────┐   ┌─────────────┐  │
			│  │Index Reg(A) │   │Data Reg (B) │  │ 
			│  └─────────────┘   └─────────────┘  │
			└─────────────────────────────────────┘
			        ▲                       ▲
			        │                       │
			   Port 0x3d4              Port 0x3d5
  ```
  - Step-by-Step Process
    - Select the VGA register to access by writing the register number to port 0x3d4. `write_byte_to_port(0x3d4,14)` for the high byte of the cursor position and `write_byte_to_port(0x3d4,15)` for the low byte.
    - Read the data from port 0x3d5. `read_byte_from_port(0x3d5)`
    - The offset from base vga address is calculated as `((high_byte << 8) | low_byte) * 2 `
> Note : `in` and `out` instructions are used to communicate via ports