// Read a byte from specified port
unsigned char read_byte_from_port(unsigned short port){
	unsigned char result;
	__asm__ ("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
} 

// Write a byte to specified port
void write_byte_to_port(unsigned short port,unsigned char data){
	__asm__ ("out %%al, %%dx" : : "a" (data),"d" (port));
}

// Read a word from specified port
unsigned short read_word_from_port(unsigned short port){
	unsigned short result;
	__asm__ ("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

// Write a word to specified port
void write_word_to_port(unsigned short port,unsigned short data){
	__asm__ ("out %%ax, %%dx" : : "a" (data),"d" (port));
}