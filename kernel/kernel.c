#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../drivers/shell.h"

void kernel_init(){
	clear_screen();

	isr_install();
	irq_install();
	kprint("klutz@helios$ ");
}

int main(){
	kernel_init();
}
