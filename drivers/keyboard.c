#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../drivers/shell.h"
#include "../include/types.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT 0x2A
#define RSHIFT 0x36

static char key_buffer[256];
static int shift_pressed = 0;

#define SC_MAX 57
const char* sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
	"7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
	"R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
	"A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
	"LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
	"/", "RShift", "Keypad *", "LAlt", "Spacebar" };

const char sc_ascii[] = {
	'?', '?', '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', '?', '?',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '[', ']', '?', '?', 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', '?', '?',
	'?', ' '};

const char sc_ascii_shifted[] = {
	'?', '?', '!', '@', '#', '$', '%', '^',
	'&', '*', '(', ')', '_', '+', '?', '?',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
	'O', 'P', '{', '}', '?', '?', 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
	'"', '~', '?', '|', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', '?', '?',
	'?', ' '};

static void keyboard_callback(struct registers_t regs) {
	uint8_t scancode = port_byte_in(0x60);

	if (scancode == LSHIFT || scancode == RSHIFT) {
		shift_pressed = 1;
		return;
	}
	if (scancode == (LSHIFT + 0x80) || scancode == (RSHIFT + 0x80)) {
		shift_pressed = 0;
		return;
	}

	if (scancode > SC_MAX || scancode & 0x80) return;

	if (scancode == BACKSPACE) {
		if (strlen(key_buffer) > 0) {
			backspace(key_buffer);
			kprint_backspace();
		}
	}
	else if (scancode == ENTER) {
		kprint("\n");
		user_input(key_buffer);
		key_buffer[0] = '\0';
	}
	else {
		char letter;
		if (shift_pressed) {
			letter = sc_ascii_shifted[scancode];
		} else {
			letter = sc_ascii[scancode];
		}

		if (letter != '?') {
			char str[2] = { letter, '\0' };
			append(key_buffer, letter);
			kprint(str);
		}
	}
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, keyboard_callback);
}
