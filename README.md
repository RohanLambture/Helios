# Helios OS

[![Language](https://img.shields.io/badge/language-C%20%26%20Assembly-orange.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/platform-x86-blue.svg)](https://en.wikipedia.org/wiki/X86)

A tiny 32-bit x86 operating system built from scratch for educational purposes.

This is a minimal, educational operating system built from scratch for the x86 (i386) architecture. It features a custom bootloader, a 32-bit protected-mode kernel written in C and assembly, basic drivers, and a simple interrupt subsystem and basic shell.

> This project is not intended for production use.And not yet tested on real hardware.

---

## Features

-   **Bootloader**: Loads the kernel from disk and transitions control to 32-bit mode.
-   **Protected Mode**: GDT setup and mode switch, with a flat memory model.
-   **Interrupts**: IDT setup and basic ISR/IRQ plumbing.
-   **Screen Output**: Text-mode screen driver for logging and printing.
-   **Timer and Keyboard Interrupt Handling**: Basic interrupt handling for timer and keyboard.
-   **Ports I/O**: Minimal port helpers for device interaction.
-   **Filesystem**: Simple filesystem and disk driver.
-   **Shell**: Basic shell and command interface.
-   **Build System**: A `Makefile` using `nasm`, `clang`/`i386-elf-gcc`, and `ld.lld`.

---

## Getting Started

### Prerequisites

You will need an x86 cross-compiling toolchain and QEMU. You can use either `clang` or `i386-elf-gcc`.

-   **Required tools**: `nasm`, `clang` or `i386-elf-gcc`, `ld.lld` (with clang), `qemu-system-i386`, `gdb`

### Build and Run

-   **Build** the kernel and bootable image:

    ```bash
    make
    ```

-   **Run** in QEMU:

    ```bash
    make run
    ```

    This launches `qemu-system-i386` with the generated `os-image.bin`.

-   **Clean** build artifacts:

    ```bash
    make clean
    ```

---

## Roadmap

-   Paging and virtual memory
-   A simple memory allocator
-   An advanced disk driver
-   Support for network protocols and devices
-   Basic graphical capabilities
---

## References

-   OSDev Wiki — [OSDev.org](https://wiki.osdev.org)
