#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../drivers/shell.h"
#include "../fs/hfs.h"

void kernel_init(){
	clear_screen();

	isr_install();
	irq_install();

	kprint("           Helios OS v1.0 \n");
	kprint("==========================================\n\n");
	kprint("Initializing filesystem...\n");
	hfs_init();
	kprint("\n");
	kprint("Available commands:\n");
	kprint("  help                - Show this help\n");
	kprint("  clear               - Clear screen\n");
	kprint("  end                 - Shutdown system\n");
	kprint("  fs_init             - Initialize filesystem\n");
	kprint("  ls                  - List directory contents\n");
	kprint("  mkdir <name>...     - Create directory(ies)\n");
	kprint("  cd <name>           - Change directory\n");
	kprint("  touch <name>...     - Create empty file(s)\n");
	kprint("  write <file> <data> - Write data to file\n");
	kprint("  cat <file>          - Display file contents\n");
	kprint("  rm <name>...        - Delete file(s)/directory(ies)\n");
	kprint("  debug               - Show filesystem debug info\n");
	kprint("klutz@helios$ ");
}

int main(){
	kernel_init();
	return 0;
}
