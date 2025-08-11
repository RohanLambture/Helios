#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../kernel/util.h"

void isr_install() {
	set_idt_entry(0, (uint32_t)isr0);
	set_idt_entry(1, (uint32_t)isr1);
	set_idt_entry(2, (uint32_t)isr2);
	set_idt_entry(3, (uint32_t)isr3);
	set_idt_entry(4, (uint32_t)isr4);
	set_idt_entry(5, (uint32_t)isr5);
	set_idt_entry(6, (uint32_t)isr6);
	set_idt_entry(7, (uint32_t)isr7);
	set_idt_entry(8, (uint32_t)isr8);
	set_idt_entry(9, (uint32_t)isr9);
	set_idt_entry(10, (uint32_t)isr10);
	set_idt_entry(11, (uint32_t)isr11);
	set_idt_entry(12, (uint32_t)isr12);
	set_idt_entry(13, (uint32_t)isr13);
	set_idt_entry(14, (uint32_t)isr14);
	set_idt_entry(15, (uint32_t)isr15);
	set_idt_entry(16, (uint32_t)isr16);
	set_idt_entry(17, (uint32_t)isr17);
	set_idt_entry(18, (uint32_t)isr18);
	set_idt_entry(19, (uint32_t)isr19);
	set_idt_entry(20, (uint32_t)isr20);
	set_idt_entry(21, (uint32_t)isr21);
	set_idt_entry(22, (uint32_t)isr22);
	set_idt_entry(23, (uint32_t)isr23);
	set_idt_entry(24, (uint32_t)isr24);
	set_idt_entry(25, (uint32_t)isr25);
	set_idt_entry(26, (uint32_t)isr26);
	set_idt_entry(27, (uint32_t)isr27);
	set_idt_entry(28, (uint32_t)isr28);
	set_idt_entry(29, (uint32_t)isr29);
	set_idt_entry(30, (uint32_t)isr30);
	set_idt_entry(31, (uint32_t)isr31);

	set_idt();
}

char *exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void isr_handler(struct registers_t r) {
	kprint("received interrupt: ");
	char s[3];
	int_to_ascii(r.int_no, s);
	kprint(s);
	kprint("\n");
	kprint(exception_messages[r.int_no]);
	kprint("\n");
}
