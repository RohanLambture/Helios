#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../cpu/ports.h"
#include "../drivers/keyboard.h"
#include "timer.h"
#include "../libc/string.h"

isr_t interrupt_handlers[256];

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
	
	// Mapping PIC 
	port_byte_out(0x20, 0x11);
	port_byte_out(0xA0, 0x11);
	port_byte_out(0x21, 0x20);
	port_byte_out(0xA1, 0x28);
	port_byte_out(0x21, 0x04);
	port_byte_out(0xA1, 0x02);
	port_byte_out(0x21, 0x01);
	port_byte_out(0xA1, 0x01);
	port_byte_out(0x21, 0x0);
	port_byte_out(0xA1, 0x0); 

	// Install the IRQs
	set_idt_entry(32, (uint32_t)irq0);
	set_idt_entry(33, (uint32_t)irq1);
	set_idt_entry(34, (uint32_t)irq2);
	set_idt_entry(35, (uint32_t)irq3);
	set_idt_entry(36, (uint32_t)irq4);
	set_idt_entry(37, (uint32_t)irq5);
	set_idt_entry(38, (uint32_t)irq6);
	set_idt_entry(39, (uint32_t)irq7);
	set_idt_entry(40, (uint32_t)irq8);
	set_idt_entry(41, (uint32_t)irq9);
	set_idt_entry(42, (uint32_t)irq10);
	set_idt_entry(43, (uint32_t)irq11);
	set_idt_entry(44, (uint32_t)irq12);
	set_idt_entry(45, (uint32_t)irq13);
	set_idt_entry(46, (uint32_t)irq14);
	set_idt_entry(47, (uint32_t)irq15);

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

	if(r.int_no <=19){
		// critical exception , halt the system
		kprint("critical exception occured.System halted\n");
		asm volatile("cli");
		asm volatile("hlt");
		// __asm__ __volatile__("cli;hlt");
		// while(1);
	}
}
void register_interrupt_handler(uint8_t n, isr_t handler) {
	interrupt_handlers[n] = handler;
}

void irq_handler(struct registers_t r) {
	if (r.int_no >= 40) port_byte_out(0xA0, 0x20); //slave
	port_byte_out(0x20, 0x20); // master

	// Handling the interrupt 
	if (interrupt_handlers[r.int_no] != 0) {
		isr_t handler = interrupt_handlers[r.int_no];
		handler(r);
	}
}

void irq_install(){
	asm volatile("sti");

	init_timer(50);
	asm volatile("sti");
	init_keyboard();
}
