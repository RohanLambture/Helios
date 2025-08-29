#include "shell.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../fs/hfs.h"

#define MAX_ARGS 16

static int parse_command(char *input, char *argv[]) {
	int argc = 0;
	char *p = input;

	// Skip leading whitespace
	while (*p == ' ') {
		p++;
	}

	while (*p != '\0' && argc < MAX_ARGS) {
		argv[argc++] = p;

		// Go to the end of the argument
		while (*p != ' ' && *p != '\0') {
			p++;
		}

		// Null-terminate the argument
		if (*p == ' ') {
			*p = '\0';
			p++;
		}

		// Skip more whitespace
		while (*p == ' ') {
			p++;
		}
	}
	return argc;
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
    char *argv[MAX_ARGS];
    int argc;

    argc = parse_command(input, argv);

    if (argc == 0) {
        kprint("klutz@helios$ ");
        return;
    }

    to_upper(argv[0]);

	if (strcmp(argv[0], "END") == 0) {
		kprint("Stopping the CPU. Bye!\n");
		asm volatile("hlt");
	} else if(strcmp(argv[0], "CLEAR") == 0) {
		clear_screen();
		kprint("klutz@helios$ ");
		return;
	} else if(strcmp(argv[0], "HELP") == 0) {
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
	} else if(strcmp(argv[0], "FS-INIT") == 0) {
		hfs_init();
	} else if(strcmp(argv[0], "LS") == 0) {
		hfs_list_dir();
	} else if(strcmp(argv[0], "MKDIR") == 0) {
		if (argc < 2) {
		kprint("Usage: mkdir <directory_name>...\n");
	} else {
		for (int i = 1; i < argc; i++) {
			hfs_create_dir(argv[i]);
		}
	}
	} else if(strcmp(argv[0], "CD") == 0) {
		if(argc < 2) {
			kprint("Usage: cd <directory_name>\n");
		} else {
			hfs_change_dir(argv[1]);
		}
	} else if(strcmp(argv[0], "TOUCH") == 0) {
		if (argc < 2) {
			kprint("Usage: touch <filename>...\n");
		} else {
			for (int i = 1; i < argc; i++) {
				hfs_create_file(argv[i]);
			}
		}
	} else if(strcmp(argv[0], "WRITE") == 0) {
		if(argc < 3) {
			kprint("Usage: write <filename> <data>\n");
		} else {
			// Concatenate the rest of the arguments into the data
			char data[256] = {0};
			int len = 0;
			for (int i = 2; i < argc; i++) {
				strcat(data, argv[i]);
				if (i < argc - 1) {
				strcat(data, " ");
				}
			}
			if(hfs_write_file(argv[1], data, strlen(data)) == 0) {
				kprint("File written successfully\n");
			}
		}
	} else if(strcmp(argv[0], "CAT") == 0) {
		if(argc < 2) {
			kprint("Usage: cat <filename>\n");
		} else {
			char buffer[1024];
			uint32_t bytes_read = 0;

			if(hfs_read_file(argv[1], buffer, sizeof(buffer) - 1, &bytes_read) == 0) {
				buffer[bytes_read] = '\0';
				kprint("File contents:\n");
				kprint(buffer);
				kprint("\n");
			}
		}
	} else if(strcmp(argv[0], "RM") == 0) {
		if (argc < 2) {
			kprint("Usage: rm <filename_or_directory>...\n");
		} else {
			for (int i = 1; i < argc; i++) {
				hfs_delete(argv[i]);
			}
		}
	} else if(strcmp(argv[0], "DEBUG") == 0){
		hfs_debug_info();
	} else {
		if(strlen(input) > 0) {
			kprint("Unknown command: ");
			kprint(input);
			kprint("\nType 'help' for available commands.\n");
		}
	}
	kprint("klutz@helios$ ");
}
