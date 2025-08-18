#include "timer.h"
#include "../cpu/ports.h"
#include "isr.h"

uint32_t tick = 0;

static void timer_callback(struct registers_t regs){
	tick++;
	// kprint("Tick- ");
	//
	// char buff[256];
	// int_to_ascii(tick, buff);
	// kprint(buff);
	// kprint("\n");
}

void init_timer(uint32_t frequency){
	// set keyboard handler in interrupt array
	asm volatile("cli");
	register_interrupt_handler(IRQ0, timer_callback);

	// TODO: hardcoded value of PIT base frequency ? 
	uint32_t divisor = 1193180 / frequency;
	uint8_t low =  lower_8(divisor);
	uint8_t high = higher_8(divisor);


	port_byte_out(0x43, 0x36);
	port_byte_out(0x40, low);
	port_byte_out(0x40, high);
}
