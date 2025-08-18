#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"

int main() {
	clear_screen();
	
	isr_install();
	asm volatile("sti");
	init_timer(50);

	while(1){

	}
}
