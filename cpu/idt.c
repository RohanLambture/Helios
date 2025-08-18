#include "idt.h"

struct idt_entry idt[IDT_ENTRIES];
struct idt_register idt_reg;

void set_idt_entry(int n, uint32_t handler){
	idt[n].low_offset = lower_16(handler);
	idt[n].kernel_segment_selector = IDT_SEGMENT_SELECTOR;
	idt[n].reserved_zero = 0;
	idt[n].flags = 0x08E; // x08E ( interrupt gate) , 0x08F (trap gate) , 0x085 (Task gate)
	idt[n].high_offset = higher_16(handler);
}

void set_idt(){
	idt_reg.base = (uint32_t) &idt;
	idt_reg.limit = IDT_ENTRIES * sizeof(struct idt_entry) - 1;

	// Load the &idt reg
	__asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
