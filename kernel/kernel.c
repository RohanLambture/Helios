#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"

int main() {
	clear_screen();
	kprint("Checkpoint1\n");
	
	isr_install();
	kprint("Checkpoint2\n");
	asm volatile("sti");

	kprint("Checkpoint3\n");
	init_timer(5);

	// TODO: Fix sti issue 
	// asm volatile("sti");
	kprint("Checkpoint4\n");

	// for (int i = 0; i < 24; i++) {
	// 	char str[255];
	// 	int_to_ascii(i, str);
	// 	kprint_at(str, 0, i);
	// }

}
