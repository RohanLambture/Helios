#ifndef IDT_H
#define IDT_H

#include "../include/types.h"

#define IDT_SEGMENT_SELECTOR 0x08

struct idt_entry{
	uint16_t low_offset;
	uint16_t kernel_segment_selector;
	uint8_t reserved_zero;
	uint8_t flags; 
	uint16_t high_offset;
}__attribute__((packed));

struct idt_register{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));


#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];
struct idt_register idt_reg;

void set_idt_entry(int n,uint32_t handler);
void set_idt();

#endif
