#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"

int main() {
	clear_screen();
	
	isr_install();
	asm volatile("sti");

	kprint("kernel.c prev");
	init_timer(50);
	kprint("kernel.c post");

	while(1){
	}
}
