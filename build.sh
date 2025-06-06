#!/bin/bash

# Compiles and runs a simple operating system

set -e  # Exit on any error

echo "Building OS..."

# Compile kernel.c
echo "Compiling kernel.c..."
i386-elf-gcc -ffreestanding -c kernel.c -o kernel.o

# Assemble kernel_entry.asm
echo "Assembling kernel_entry.asm..."
nasm kernel_entry.asm -f elf -o kernel_entry.o

# Link kernel
echo "Linking kernel..."
i386-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary

# Assemble main.asm (bootloader)
echo "Assembling bootloader..."
nasm main.asm -f bin -o main.bin

# Create OS image
echo "Creating OS image..."
cat main.bin kernel.bin > os-image.bin

# Run with QEMU
echo "Starting QEMU..."
qemu-system-i386 -fda os-image.bin
