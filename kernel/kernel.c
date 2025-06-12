#include "../drivers/screen.h"
#include "util.h"

int main() {
	printk("hello world\nNext line");
	// printk_at("hello",80,30, 0x0f);
	// clear_screen(0,0);
	return 0;
}
