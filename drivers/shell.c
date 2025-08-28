#include "shell.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../fs/hfs.h"

static void parse_command(char *input, char *cmd, char *arg1, char *arg2) {
	int i = 0, j = 0;

	// Clear buffers
	for(int k = 0; k < 32; k++) {
		cmd[k] = '\0';
		arg1[k] = '\0';
		arg2[k] = '\0';
	}

	// Skip leading spaces
	while(input[i] == ' ') i++;

	// Extract command
	while(input[i] != ' ' && input[i] != '\0' && j < 31) {
		cmd[j++] = input[i++];
	}
	cmd[j] = '\0';

	// Skip spaces
	while(input[i] == ' ') i++;

	// Extract first argument
	j = 0;
	while(input[i] != ' ' && input[i] != '\0' && j < 31) {
		arg1[j++] = input[i++];
	}
	arg1[j] = '\0';

	// Skip spaces
	while(input[i] == ' ') i++;

	// Extract second argument (rest of the line)
	j = 0;
	while(input[i] != '\0' && j < 255) {
		arg2[j++] = input[i++];
	}
	arg2[j] = '\0';
}

// Convert string to uppercase for case-insensitive comparison
static void to_upper(char *str) {
	for(int i = 0; str[i] != '\0'; i++) {
		if(str[i] >= 'a' && str[i] <= 'z') {
		str[i] = str[i] - 'a' + 'A';
		}
	}
}

void user_input(char *input) {
	char cmd[32], arg1[32], arg2[256];

	parse_command(input, cmd, arg1, arg2);
	to_upper(cmd);

	if (strcmp(cmd, "END") == 0) {
		kprint("Stopping the CPU. Bye!\n");
		asm volatile("hlt");
	} else if(strcmp(cmd, "CLEAR") == 0) {
		clear_screen();
		kprint("klutz@helios$ ");
		return;
	} else if(strcmp(cmd, "HELP") == 0) {
		kprint("Available commands:\n");
		kprint("  help          - Show this help\n");
		kprint("  clear         - Clear screen\n");
		kprint("  end           - Shutdown system\n");
		kprint("  fs_init       - Initialize filesystem\n");
		kprint("  ls            - List directory contents\n");
		kprint("  mkdir <name>  - Create directory\n");
		kprint("  cd <name>     - Change directory\n");
		kprint("  touch <name>  - Create empty file\n");
		kprint("  write <file> <data> - Write data to file\n");
		kprint("  cat <file>    - Display file contents\n");
		kprint("  rm <name>     - Delete file/directory\n");
	} else if(strcmp(cmd, "FS_INIT") == 0) {
		hfs_init();
	} else if(strcmp(cmd, "LS") == 0) {
		hfs_list_dir();
	} else if(strcmp(cmd, "MKDIR") == 0) {
		if(strlen(arg1) == 0) {
			kprint("Usage: mkdir <directory_name>\n");
		} else {
			hfs_create_dir(arg1);
		}
	} else if(strcmp(cmd, "CD") == 0) {
		if(strlen(arg1) == 0) {
			kprint("Usage: cd <directory_name>\n");
		} else {
			hfs_change_dir(arg1);
		}
	} else if(strcmp(cmd, "TOUCH") == 0) {
		if(strlen(arg1) == 0) {
			kprint("Usage: touch <filename>\n");
		} else {
			hfs_create_file(arg1);
		}
	} else if(strcmp(cmd, "WRITE") == 0) {
		if(strlen(arg1) == 0 || strlen(arg2) == 0) {
			kprint("Usage: write <filename> <data>\n");
		} else {
			if(hfs_write_file(arg1, arg2, strlen(arg2)) == 0) {
				kprint("File written successfully\n");
			}
		}
	} else if(strcmp(cmd, "CAT") == 0) {
		if(strlen(arg1) == 0) {
			kprint("Usage: cat <filename>\n");
		} else {
			char buffer[1024];
			uint32_t bytes_read = 0;

			if(hfs_read_file(arg1, buffer, sizeof(buffer) - 1, &bytes_read) == 0) {
				buffer[bytes_read] = '\0';
				kprint("File contents:\n");
				kprint(buffer);
				kprint("\n");
			}
		}
	} else if(strcmp(cmd, "RM") == 0) {
		if(strlen(arg1) == 0) {
			kprint("Usage: rm <filename_or_directory>\n");
		} else {
			hfs_delete(arg1);
		}
	} else {
		if(strlen(input) > 0) {
			kprint("Unknown command: ");
			kprint(input);
			kprint("\nType 'help' for available commands.\n");
		}
	}
	kprint("klutz@helios$ ");
}
