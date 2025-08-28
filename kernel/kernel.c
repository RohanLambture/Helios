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
	kprint("Commands available:\n");
	kprint("  help      - Show available commands\n");
	kprint("  clear     - Clear screen\n");
	kprint("  fs_init   - Initialize filesystem\n");
	kprint("  ls        - List directory contents\n");
	kprint("  mkdir <name>  - Create directory\n");
	kprint("  cd <name>     - Change directory\n");
	kprint("  touch <file>  - Create empty file\n");
	kprint("  write <file> <data> - Write to file\n");
	kprint("  cat <file>    - Display file\n");
	kprint("  rm <name>     - Delete file/dir\n\n");
	kprint("klutz@helios$ ");
}

int main(){
	kernel_init();
	return 0;
}
