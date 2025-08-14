
A toy operating system built for fun and learning.

## About

This project is a simple OS built from scratch.

## Features

*   **Bootloader:** Loads the kernel from disk.
*   **32-bit Protected Mode:** Switches the CPU from real mode to 32-bit protected mode.
*   **Screen Driver:** Basic driver for printing to the screen.
*   **Interrupt Handling:** Basic interrupt handling is set up.
*   **Memory Management:** The project uses a flat memory model.


## Building and Running

To build the OS, you'll need `nasm`, `clang` , `ld.lld` and `qemu`.

```bash
make
```

To run the OS in QEMU:

```bash
make run
```

## Debugging

To debug the OS with GDB:

```bash
make debug
```

This will start a GDB server in QEMU. You can then connect to it with GDB.

## Cleaning

To clean up the build files:

```bash
make clean
```

## Future Work

*   Implement a simple filesystem.
*   Create a basic shell.
