#include "../drivers/ports.h"

int main() {
	write_byte_to_port(0x3d4, 14);
	int position = read_byte_from_port(0x3d5);
	position = position << 8;
	write_byte_to_port(0x3d4, 15);
	position += read_byte_from_port(0x3d5);
	
	int offset = position * 2;
	char *vga = (char*)0xb8000;
	vga[offset] = 'x';
	vga[offset + 1] = 0x0f;
	return 0;
}
