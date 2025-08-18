#include "../drivers/screen.h"
#include "../cpu/isr.h"

void kernel_init(){
	clear_screen();
	kprint("Starting Helios\n");

	isr_install();
	irq_install();
}
int main() {
	kernel_init();
}
