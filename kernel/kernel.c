#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../drivers/shell.h"

void kernel_init(){
	clear_screen();

	isr_install();
	irq_install();
	kprint("           Helios OS v1.0 \n");
	kprint("==========================================\n\n");
	kprint("Commands available:\n");
	kprint("  help   - Show this help\n");
	kprint("  clear  - Clear screen\n\n");
	kprint("klutz@helios$ ");
}

int main(){
	kernel_init();
}
