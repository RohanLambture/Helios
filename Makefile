# Cross-compiler settings
CC = i386-elf-gcc
LD = i386-elf-ld
ASM = nasm

# Compiler flags
CFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -fno-stack-protector -fno-builtin -nostdlib
LDFLAGS = --oformat binary
ASMFLAGS = -f elf

# Directories
BOOT_DIR = boot
KERNEL_DIR = kernel
DRIVERS_DIR = drivers
BUILD_DIR = build
IMAGE_NAME = os-image.bin

# Memory layout (must match boot/main.asm)
KERNEL_OFFSET = 0x1000


.PHONY: all clean run debug help disasm

# Default target
all: $(IMAGE_NAME)

# Create the complete OS image
$(IMAGE_NAME): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	@echo "Creating OS image..."
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(IMAGE_NAME)
	@echo "✓ OS image created: $(IMAGE_NAME)"

# Compile the bootloader
$(BUILD_DIR)/boot.bin: $(BOOT_DIR)/main.asm $(BOOT_DIR)/*.asm | $(BUILD_DIR)
	@echo "Assembling bootloader..."
	$(ASM) -f bin -I $(BOOT_DIR)/ $(BOOT_DIR)/main.asm -o $(BUILD_DIR)/boot.bin
	@echo "✓ Bootloader compiled"

# Link the kernel
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/util.o $(BUILD_DIR)/ports.o $(BUILD_DIR)/screen.o | $(BUILD_DIR)
	@echo "Linking kernel..."
	$(LD) -o $(BUILD_DIR)/kernel.bin -Ttext $(KERNEL_OFFSET) $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/util.o $(BUILD_DIR)/ports.o $(BUILD_DIR)/screen.o $(LDFLAGS)
	@echo "✓ Kernel linked"

# Compile kernel entry point
$(BUILD_DIR)/kernel_entry.o: $(BOOT_DIR)/kernel_entry.asm | $(BUILD_DIR)
	@echo "Assembling kernel entry..."
	$(ASM) $(ASMFLAGS) $(BOOT_DIR)/kernel_entry.asm -o $(BUILD_DIR)/kernel_entry.o

# Compile kernel C code
$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c | $(BUILD_DIR)
	@echo "Compiling kernel..."
	$(CC) $(CFLAGS) -I$(DRIVERS_DIR) -c $(KERNEL_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o

# Compile kernel utility functions
$(BUILD_DIR)/util.o: $(KERNEL_DIR)/util.c | $(BUILD_DIR)
	@echo "Compiling kernel utilities..."
	$(CC) $(CFLAGS) -I$(DRIVERS_DIR) -c $(KERNEL_DIR)/util.c -o $(BUILD_DIR)/util.o

# Compile driver C code - ports
$(BUILD_DIR)/ports.o: $(DRIVERS_DIR)/ports.c | $(BUILD_DIR)
	@echo "Compiling driver: ports.c..."
	$(CC) $(CFLAGS) -c $(DRIVERS_DIR)/ports.c -o $(BUILD_DIR)/ports.o

# Compile driver C code - screen
$(BUILD_DIR)/screen.o: $(DRIVERS_DIR)/screen.c | $(BUILD_DIR)
	@echo "Compiling driver: screen.c..."
	$(CC) $(CFLAGS) -c $(DRIVERS_DIR)/screen.c -o $(BUILD_DIR)/screen.o

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Run the OS in QEMU
run: $(IMAGE_NAME)
	@echo "Starting QEMU..."
	qemu-system-i386 -fda $(IMAGE_NAME)

# Debug with QEMU (with monitor and debugging options)
debug: $(IMAGE_NAME)
	@echo "Starting QEMU in debug mode..."
	qemu-system-i386 -fda $(IMAGE_NAME) -monitor stdio -d cpu_reset,guest_errors

# Disassemble the kernel
disasm: $(BUILD_DIR)/kernel.bin
	@echo "Disassembling kernel..."
	ndisasm -b 32 $(BUILD_DIR)/kernel.bin

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	rm -f $(IMAGE_NAME)
	@echo "✓ Clean complete"

# Show help information
help:
	@echo "Flubix OS Build System"
	@echo "====================="
	@echo ""
	@echo "Available targets:"
	@echo "  all     - Build the complete OS image (default)"
	@echo "  run     - Build and run the OS in QEMU"
	@echo "  debug   - Build and run with QEMU debugging"
	@echo "  disasm  - Disassemble the kernel binary"
	@echo "  clean   - Remove all build artifacts"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Build components:"
	@echo "  $(IMAGE_NAME)           - Complete OS image"
	@echo "  $(BUILD_DIR)/boot.bin          - Bootloader binary"
	@echo "  $(BUILD_DIR)/kernel.bin        - Kernel binary"
	@echo ""
	@echo "Configuration:"
	@echo "  Kernel offset: $(KERNEL_OFFSET)"
	@echo "  Compiler:      $(CC)"
	@echo "  Assembler:     $(ASM)"