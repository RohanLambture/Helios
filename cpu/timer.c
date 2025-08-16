#include "timer.h"
#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "isr.h"

uint32_t tick = 0;

static void timer_callback(struct registers_t regs){
	tick++;
	kprint("Tick- ");
	
	char buff[256];
	int_to_ascii(tick, buff);
	kprint(buff);
	kprint("\n");
}

void init_timer(uint32_t frequency){
	// set keyboard handler in interrupt array
	register_interrupt_handler(IRQ0, timer_callback);

	// TODO: better way to handle this
	if(frequency == 0) return;

	// TODO: hardcoded value of PIT base frequency ? 
	uint32_t divisor = 1193180 / frequency;
	uint8_t low =  lower_8(divisor);
	uint8_t high = higher_8(divisor);

	asm volatile ("cli");

	port_byte_out(0x43, 0x36);
	port_byte_out(0x40, low);
	port_byte_out(0x40, high);

	kprint("Checkpoint3.1\n");
	// asm volatile ("sti");
}
