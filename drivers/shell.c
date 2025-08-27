#include "shell.h"
#include "../drivers/screen.h"
#include "../libc/string.h"

void user_input(char *input) {
	if (strcmp(input, "END") == 0) {
		kprint("Stopping the CPU. Bye!\n");
		asm volatile("hlt");
	}else if(strcmp(input,"CLEAR") == 0){
		clear_screen();
		kprint("klutz@helios$ ");
		return ;
	}else if(strcmp(input,"HELP") == 0){
		kprint("Commands available:\n");
		kprint("  help   - Show this help\n");
		kprint("  clear  - Clear screen\n\n");
		kprint("klutz@helios$ ");
		return ;
	}
	kprint("You said: ");
	kprint(input);
	kprint("\n");
	kprint("klutz@helios$ ");
}
