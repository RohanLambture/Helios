# TODO: Fix clang toolchain

# C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
# ASM_SOURCES = $(wildcard cpu/*.asm)
# HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)
# OBJ = ${C_SOURCES:.c=.o} ${ASM_SOURCES:.asm=.o}
#
# # Clang configuration
# CC = clang
# LINKER = ld.lld
# GDB = gdb
# CFLAGS = -g --target=i386-elf -ffreestanding -fno-builtin -nostdlib -nostdinc
#
# # First rule is run by default
# os-image.bin: boot/bootsect.bin kernel.bin
# 	cat $^ > os-image.bin
#
# # '--oformat binary' del't need
# # to 'strip' them manually on this case
# kernel.bin: ${OBJ}
# 	${LINKER} -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
#
# # Used for debugging purposes
# kernel.elf: ${OBJ}
# 	${LINKER} -m elf_i386 -o $@ -Ttext 0x1000 $^ 
#
# run: os-image.bin
# 	qemu-system-i386 -fda os-image.bin
#
# # Open the connection to qemu and load our kernel-object file with symbols
# debug: os-image.bin kernel.elf
# 	qemu-system-i386 -s -fda os-image.bin &
# 	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
#
# # Generic rules for wildcards
# # To make an object, always compile from its .c
# %.o: %.c ${HEADERS}
# 	${CC} ${CFLAGS} -c $< -o $@
#
# # Only for CPU assembly files (not boot sector)
# cpu/%.o: cpu/%.asm
# 	nasm $< -f elf -o $@
#
# # Boot sector needs binary format
# boot/bootsect.bin: boot/bootsect.asm
# 	nasm $< -f bin -o $@
#
# clean:
# 	rm -rf *.bin *.dis *.o os-image.bin *.elf
# 	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o

# GCC toolchain
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h include/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

# Change this if your cross-compiler is somewhere else
CC = i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g 

# First rule is run by default
os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o
