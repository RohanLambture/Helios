### Helios — A tiny 32‑bit x86 OS

This is a minimal, educational operating system built from scratch for the x86 (i386) architecture. It features a handcrafted bootloader, a 32‑bit protected‑mode kernel written in C and assembly, basic drivers, and a simple interrupt subsystem. 

> This project is not intended for production use.

---

### Features

- **Bootloader**: Loads the kernel from disk and transitions control to 32‑bit mode
- **Protected Mode**: GDT setup and mode switch, flat memory model
- **Interrupts**: IDT setup, basic ISR/IRQ plumbing
- **Screen Output**: Text‑mode screen driver for logging/printing
- **Ports I/O**: Minimal port helpers for device interaction
- **Build System**: `Makefile` with `nasm`, `clang`/`i386-elf-gcc` and `ld.lld`; QEMU for emulation

---

### Prerequisites

You will need an x86 cross‑compiling toolchain and QEMU. You can use either `clang` or `i386-elf-gcc`.

- **Required tools**: `nasm`, `clang` or `i386-elf-gcc`, `ld.lld` (with clang), `qemu-system-i386`, `gdb`

---

### Build and run

- **Build** the kernel and bootable image:

```bash
make
```

- **Run** in QEMU:

```bash
make run
```

This launches `qemu-system-i386` with the generated `os-image.bin` as a floppy image.

- **Debug** with GDB:

```bash
make debug
```

This starts QEMU with a GDB server. In another terminal you can attach with:

```bash
gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
```

- **Clean** build artifacts:

```bash
make clean
```

---

### Roadmap

- Implement a simple filesystem and disk driver
- Create a basic shell and command interface
- Paging and virtual memory
- Timer and keyboard interrupt handling
- Simple memory allocator

---

### References
- OSDev Wiki — [OSDev.org](https://wiki.osdev.org)
---
